// matth-x/ESP8266-OCPP
// Copyright Matthias Akstaller 2019 - 2020
// MIT License

#ifndef CHARGEPOINTSTATUSSERVICE_H
#define CHARGEPOINTSTATUSSERVICE_H

#include <WebSocketsClient.h>

enum class ChargePointStatus {
  Available,
  Preparing,
  Charging,
  SuspendedEVSE,
  SuspendedEV,
  Finishing,    //not supported by this client
  Reserved,     //not supported by this client
  Unavailable,  //not supported by this client
  Faulted,      //not supported by this client
  NOT_SET //not part of OCPP 1.6
};

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
