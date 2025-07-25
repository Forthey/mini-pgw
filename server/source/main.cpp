#include "logger/LoggerFactory.h"


int main() {
    auto logger = server::LoggerFactory::getSingletonLogger("MainLogger");

    logger->debug("Hello World!", WITH_CONTEXT);
}