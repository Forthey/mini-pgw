#include "logger/LoggerFactory.h"


int main() {
    auto logger = server::LoggerFactory::getSingletonLogger("MainLogger");

    logger->error("Hello World!", WITH_CONTEXT);
}