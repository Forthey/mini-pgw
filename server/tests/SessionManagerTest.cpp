#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <chrono>
#include "session_manager/SessionManager.h"
#include "MockLogger.h"

using namespace server;
using namespace testing;
using namespace std::chrono_literals;

class SessionManagerTest : public Test {
protected:
    std::unique_ptr<SessionManager> session_manager;
    std::shared_ptr<MockLogger> mock_logger = std::make_shared<MockLogger>();
    std::string cdr_file_path;
    const int TTL = 1;
    const int SHUTDOWN_RATE = 10;

    void SetUp() override {
        cdr_file_path = std::filesystem::temp_directory_path() / "cdr_test.txt";
        session_manager = std::make_unique<SessionManager>(TTL, SHUTDOWN_RATE, cdr_file_path, mock_logger);
    }

    void TearDown() override {
        if (session_manager) {
            session_manager->shutdown(false);
            session_manager.reset();
        }
        if (std::filesystem::exists(cdr_file_path)) {
            std::filesystem::remove(cdr_file_path);
        }
    }

    std::vector<std::string> readCDRFile() const {
        std::ifstream file(cdr_file_path);
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        return lines;
    }
};

// Тест создания и удаления сессии
TEST_F(SessionManagerTest, CreateAndRetrieveSession) {
    EXPECT_TRUE(session_manager->upsertSession("123456789"));

    auto session = session_manager->getSession("123456789");
    EXPECT_TRUE(session.has_value());

    EXPECT_GT(session->expiration_timestamp, Clock::now());
}

// Тест черного списка
TEST_F(SessionManagerTest, BlacklistedSession) {
    session_manager->setBlacklist({"123456789"});

    EXPECT_FALSE(session_manager->upsertSession("123456789"));
    EXPECT_TRUE(session_manager->isBlacklisted("123456789"));
}

// Тест обновления сессии
TEST_F(SessionManagerTest, ExtendSession) {
    EXPECT_TRUE(session_manager->upsertSession("123456789"));
    auto firstExpiry = session_manager->getSession("123456789")->expiration_timestamp;

    std::this_thread::sleep_for(100ms);
    EXPECT_TRUE(session_manager->upsertSession("123456789"));
    auto secondExpiry = session_manager->getSession("123456789")->expiration_timestamp;

    EXPECT_GT(secondExpiry, firstExpiry);
}

// Тест истечения срока сессии
TEST_F(SessionManagerTest, SessionExpiration) {
    EXPECT_TRUE(session_manager->upsertSession("123456789"));

    std::this_thread::sleep_for(3000ms);

    EXPECT_FALSE(session_manager->getSession("123456789").has_value());

    auto cdrLines = readCDRFile();
    ASSERT_EQ(cdrLines.size(), 2);
    EXPECT_NE(cdrLines[0].find("created"), std::string::npos);
    EXPECT_NE(cdrLines[1].find("expired"), std::string::npos);
}

// Тест Graceful Shutdown
TEST_F(SessionManagerTest, GracefulShutdown) {
    session_manager->upsertSession("111");
    session_manager->upsertSession("222");

    session_manager->shutdown(true);

    std::this_thread::sleep_for(4000ms);

    EXPECT_FALSE(session_manager->getSession("111").has_value());

    auto cdrLines = readCDRFile();
    EXPECT_EQ(cdrLines.size(), 4);
}

// Тест Non-Graceful Shutdown
TEST_F(SessionManagerTest, NonGracefulShutdown) {
    session_manager->upsertSession("111");

    session_manager->shutdown(false);
    EXPECT_FALSE(session_manager->getSession("111").has_value());
}

// Тест CDR записи
TEST_F(SessionManagerTest, CDRWriting) {
    session_manager->setBlacklist({"blacklisted_imsi"});

    session_manager->upsertSession("123");
    session_manager->upsertSession("123");
    session_manager->upsertSession("blacklisted_imsi");

    auto cdrLines = readCDRFile();
    ASSERT_EQ(cdrLines.size(), 3);
    EXPECT_NE(cdrLines[0].find("created"), std::string::npos);
    EXPECT_NE(cdrLines[1].find("extended"), std::string::npos);
    EXPECT_NE(cdrLines[2].find("rejected"), std::string::npos);
}
