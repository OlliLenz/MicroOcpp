// matth-x/MicroOcpp
// Copyright Matthias Akstaller 2019 - 2023
// MIT License

#include <MicroOcpp/Model/Model.h>
#include <MicroOcpp/Model/Transactions/TransactionStore.h>
#include <MicroOcpp/Model/SmartCharging/SmartChargingService.h>
#include <MicroOcpp/Model/ConnectorBase/ConnectorsCommon.h>
#include <MicroOcpp/Model/Metering/MeteringService.h>
#include <MicroOcpp/Model/FirmwareManagement/FirmwareService.h>
#include <MicroOcpp/Model/Diagnostics/DiagnosticsService.h>
#include <MicroOcpp/Model/Heartbeat/HeartbeatService.h>
#include <MicroOcpp/Model/Authorization/AuthorizationService.h>
#include <MicroOcpp/Model/Reservation/ReservationService.h>
#include <MicroOcpp/Model/Boot/BootService.h>
#include <MicroOcpp/Model/Reset/ResetService.h>

#include <MicroOcpp/Debug.h>

using namespace MicroOcpp;

Model::Model(uint16_t bootNr) : bootNr(bootNr) {
    
}

Model::~Model() = default;

void Model::loop() {

    if (bootService) {
        bootService->loop();
    }

    if (!runTasks) {
        return;
    }

    for (auto& connector : connectors) {
        connector.loop();
    }

    if (chargeControlCommon)
        chargeControlCommon->loop();
    
    if (smartChargingService)
        smartChargingService->loop();
    
    if (heartbeatService)
        heartbeatService->loop();
    
    if (meteringService)
        meteringService->loop();
    
    if (diagnosticsService)
        diagnosticsService->loop();
    
    if (firmwareService)
        firmwareService->loop();
    
    if (reservationService)
        reservationService->loop();
    
    if (resetService)
        resetService->loop();
}

void Model::setTransactionStore(std::unique_ptr<TransactionStore> ts) {
    transactionStore = std::move(ts);
}

TransactionStore *Model::getTransactionStore() {
    return transactionStore.get();
}

void Model::setSmartChargingService(std::unique_ptr<SmartChargingService> scs) {
    smartChargingService = std::move(scs);
}

SmartChargingService* Model::getSmartChargingService() const {
    return smartChargingService.get();
}

void Model::setConnectorsCommon(std::unique_ptr<ConnectorsCommon> ccs) {
    chargeControlCommon = std::move(ccs);
}

ConnectorsCommon *Model::getConnectorsCommon() {
    return chargeControlCommon.get();
}

void Model::setConnectors(std::vector<Connector>&& connectors) {
    this->connectors = std::move(connectors);
}

unsigned int Model::getNumConnectors() const {
    return connectors.size();
}

Connector *Model::getConnector(unsigned int connectorId) {
    if (connectorId >= connectors.size()) {
        MOCPP_DBG_ERR("connector with connectorId %u does not exist", connectorId);
        return nullptr;
    }

    return &connectors[connectorId];
}

void Model::setMeteringSerivce(std::unique_ptr<MeteringService> ms) {
    meteringService = std::move(ms);
}

MeteringService* Model::getMeteringService() const {
    return meteringService.get();
}

void Model::setFirmwareService(std::unique_ptr<FirmwareService> fws) {
    firmwareService = std::move(fws);
}

FirmwareService *Model::getFirmwareService() const {
    return firmwareService.get();
}

void Model::setDiagnosticsService(std::unique_ptr<DiagnosticsService> ds) {
    diagnosticsService = std::move(ds);
}

DiagnosticsService *Model::getDiagnosticsService() const {
    return diagnosticsService.get();
}

void Model::setHeartbeatService(std::unique_ptr<HeartbeatService> hs) {
    heartbeatService = std::move(hs);
}

void Model::setAuthorizationService(std::unique_ptr<AuthorizationService> as) {
    authorizationService = std::move(as);
}

AuthorizationService *Model::getAuthorizationService() {
    return authorizationService.get();
}

void Model::setReservationService(std::unique_ptr<ReservationService> rs) {
    reservationService = std::move(rs);
}

ReservationService *Model::getReservationService() {
    return reservationService.get();
}

void Model::setBootService(std::unique_ptr<BootService> bs){
    bootService = std::move(bs);
}

BootService *Model::getBootService() const {
    return bootService.get();
}

void Model::setResetService(std::unique_ptr<ResetService> rs) {
    this->resetService = std::move(rs);
}

ResetService *Model::getResetService() const {
    return resetService.get();
}

Clock& Model::getClock() {
    return clock;
}

uint16_t Model::getBootNr() {
    return bootNr;
}
