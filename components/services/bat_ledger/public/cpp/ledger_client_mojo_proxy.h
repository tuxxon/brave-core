/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_SERVICES_BAT_LEDGER_PUBLIC_CPP_LEDGER_CLIENT_MOJO_PROXY_H_
#define BRAVE_COMPONENTS_SERVICES_BAT_LEDGER_PUBLIC_CPP_LEDGER_CLIENT_MOJO_PROXY_H_

#include <map>

#include "bat/ledger/ledger_client.h"
#include "brave/components/services/bat_ledger/public/interfaces/bat_ledger.mojom.h"

namespace bat_ledger {

class LedgerClientMojoProxy : public mojom::BatLedgerClient,
                              public ledger::LedgerCallbackHandler {
 public:
  LedgerClientMojoProxy(ledger::LedgerClient* ledger_client);
  ~LedgerClientMojoProxy() override;

  // bat_ledger::mojom::BatLedgerClient
  void GenerateGUID(GenerateGUIDCallback callback) override;
  void LoadLedgerState(LoadLedgerStateCallback callback) override;
  void OnWalletInitialized(int32_t result) override;
  void LoadPublisherState(LoadPublisherStateCallback callback) override;
  void LoadPublisherList(LoadPublisherListCallback callback) override;
  void SaveLedgerState(const std::string& ledger_state,
      SaveLedgerStateCallback callback) override;
  void SavePublisherState(const std::string& publisher_state,
      SavePublisherStateCallback callback) override;
  void SavePublishersList(const std::string& publishers_list,
      SavePublishersListCallback callback) override;

  // ledger::LedgerCallbackHandler
  void OnLedgerStateLoaded(ledger::Result result,
      const std::string& data) override;
  void OnPublisherStateLoaded(ledger::Result result,
      const std::string& data) override;
  void OnPublisherListLoaded(ledger::Result result,
      const std::string& data) override;
  void OnLedgerStateSaved(ledger::Result result) override;
  void OnPublisherStateSaved(ledger::Result result) override;
  void OnPublishersListSaved(ledger::Result result) override;

 private:
  ledger::LedgerClient* ledger_client_;
  LoadLedgerStateCallback load_ledger_state_callback_;
  LoadPublisherStateCallback load_publisher_state_callback_;
  LoadPublisherListCallback load_publisher_list_callback_;
  SaveLedgerStateCallback save_ledger_state_callback_;
  SavePublisherStateCallback save_publisher_state_callback_;
  SavePublishersListCallback save_publishers_list_callback_;

  DISALLOW_COPY_AND_ASSIGN(LedgerClientMojoProxy);
};

} // namespace bat_ledger

#endif  // BRAVE_COMPONENTS_SERVICES_BAT_LEDGER_PUBLIC_CPP_LEDGER_CLIENT_MOJO_PROXY_H_
