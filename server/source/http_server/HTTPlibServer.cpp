#include "HTTPlibServer.h"

#include <format>
#include <utility>

#include "models/http/HTTPExceptionResponse.h"


namespace server {
    void HTTPlibServer::buildHTTPException(httplib::Response &res, std::uint16_t status_code,
                                           std::string const &detail) {
        res.status = status_code;
        HTTPExceptionResponse const response_model = {
            .detail = detail,
        };

        res.set_content(HTTPExceptionResponseParser().serialize(response_model), "application/json");
    }

    void HTTPlibServer::checkSubscriber(httplib::Request const &req, httplib::Response &res) const {
        if (not req.has_param("imsi")) {
            buildHTTPException(res, 422, "Query param \"imsi\" not found");
            return;
        }

        if (session_manager_->getSession(req.get_param_value("imsi")).has_value()) {
            res.set_content("active", "text/plain");
        } else {
            res.set_content("not active", "text/plain");
        }
    }

    void HTTPlibServer::stop(httplib::Request const &req, httplib::Response &res) {
        res.status = 200;
        std::thread(shutdown_callback_).detach();
    }

    HTTPlibServer::HTTPlibServer(std::string host, std::uint16_t port, std::shared_ptr<ISessionManager> session_manager,
                                 ShutdownFunc shutdown_callback, std::shared_ptr<ILogger> logger)
        : host_(std::move(host)), port_(port), session_manager_(std::move(session_manager)),
          shutdown_callback_(std::move(shutdown_callback)), logger_(std::move(logger)) {
    }

    HTTPlibServer::~HTTPlibServer() {
        server_.stop();
    }

    bool HTTPlibServer::setup() {
        server_.Get("/check_subscriber", [this](const httplib::Request &req, httplib::Response &res) {
            this->checkSubscriber(req, res);
        });
        server_.Post("/stop", [this](const httplib::Request &req, httplib::Response &res) {
            this->stop(req, res);
        });

        server_.set_post_routing_handler([this](const httplib::Request &req, httplib::Response &res) {
            logger_->info(
                std::format("{} {} --> {}", req.method, req.path, res.status)
            );
        });

        return true;
    }

    void HTTPlibServer::listen() {
        logger_->info(
            std::format("HTTP server is listening on {}:{}", host_, port_), WITH_CONTEXT
        );
        server_.listen(host_, port_);
    }

    void HTTPlibServer::shutdown() {
        server_.stop();
    }
} // namespace server
