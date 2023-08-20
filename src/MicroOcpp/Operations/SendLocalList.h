// matth-x/MicroOcpp
// Copyright Matthias Akstaller 2019 - 2023
// MIT License

#ifndef SENDLOCALLIST_H
#define SENDLOCALLIST_H

#include <MicroOcpp/Core/Operation.h>

namespace MicroOcpp {

class Model;

namespace Ocpp16 {

class SendLocalList : public Operation {
private:
    Model& model;
    const char *errorCode = nullptr;
    bool updateFailure = true;
    bool versionMismatch = false;
public:
    SendLocalList(Model& model);

    ~SendLocalList();

    const char* getOperationType();

    void processReq(JsonObject payload);

    std::unique_ptr<DynamicJsonDocument> createConf();

    const char *getErrorCode() {return errorCode;}
};

} //end namespace Ocpp16
} //end namespace MicroOcpp
#endif
