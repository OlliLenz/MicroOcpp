// matth-x/ESP8266-OCPP
// Copyright Matthias Akstaller 2019 - 2020
// MIT License

#ifndef CHARGEPOINTSTATUSSERVICE_H
#define CHARGEPOINTSTATUSSERVICE_H

#include <WebSocketsClient.h>
#include <LinkedList.h>

#include "Variants.h"
#ifdef MULTIPLE_CONN
#include "ConnectorStatus.h"

class ChargePointStatusService {
private:
  WebSocketsClient *webSocket;
  const int numConnectors;
  ConnectorStatus **connectors;

  boolean authorized = false;
  String idTag = String('\0');

public:
  ChargePointStatusService(WebSocketsClient *webSocket, int numConnectors);

  ~ChargePointStatusService();
  
  void loop();

  void authorize(String &idTag);
  void authorize();
  void boot();
  String &getUnboundIdTag();
  boolean existsUnboundAuthorization();
  void bindAuthorization(String &idTag, int connectorId);

  ConnectorStatus *getConnector(int connectorId);
  int getNumConnectors();
};

#else

class ChargePointStatusService {
private:
  bool authorized = false;
  String idTag = String('\0');
  bool transactionRunning = false;
  int transactionId = -1;
  bool evDrawsEnergy = false;
  bool evseOffersEnergy = false;
  ChargePointStatus currentStatus = ChargePointStatus::NOT_SET;
  WebSocketsClient *webSocket;
public:
  ChargePointStatusService(WebSocketsClient *webSocket);
  void authorize(String &idTag);
  void authorize();
  void unauthorize();
  String &getIdTag();
  void startTransaction(int transactionId);
  void stopTransaction();
  int getTransactionId();
  void boot();
  void startEvDrawsEnergy();
  void stopEvDrawsEnergy();
  void startEnergyOffer();
  void stopEnergyOffer();

  void loop();

  ChargePointStatus inferenceStatus();
};

#endif
#endif
