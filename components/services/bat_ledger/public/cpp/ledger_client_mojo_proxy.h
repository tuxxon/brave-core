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
  void OnGrantCaptcha(const std::string& image,
      const std::string& hint) override;
  void OnReconcileComplete(int32_t result, const std::string& viewing_id,
      int32_t category, const std::string& probi) override;

  void LoadPublisherState(LoadPublisherStateCallback callback) override;
  void LoadPublisherList(LoadPublisherListCallback callback) override;
  void SaveLedgerState(const std::string& ledger_state,
      SaveLedgerStateCallback callback) override;
  void SavePublisherState(const std::string& publisher_state,
      SavePublisherStateCallback callback) override;
  void SavePublishersList(const std::string& publishers_list,
      SavePublishersListCallback callback) override;
  void LoadURL(const std::string& url,
      const std::vector<std::string>& headers,
      const std::string& content,
      const std::string& contentType,
      int32_t method,
      LoadURLCallback callback) override;

  void SetTimer(uint64_t time_offset, SetTimerCallback callback) override;
  void OnExcludedSitesChanged() override;
  void SaveContributionInfo(const std::string& probi, int32_t month,
      int32_t year, uint32_t date, const std::string& publisher_key,
      int32_t category) override;
  void SaveMediaPublisherInfo(const std::string& media_key,
      const std::string& publisher_id) override;
  void FetchWalletProperties() override;
  void FetchGrant(const std::string& lang,
      const std::string& payment_id) override;
  void GetGrantCaptcha() override;

  void URIEncode(const std::string& value,
      URIEncodeCallback callback) override;

  void SetContributionAutoInclude(const std::string& publisher_key,
      bool excluded, uint64_t window_id) override;

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
  void OnURLRequestResponse(uint64_t request_id,
      const std::string& url,
      int response_code,
      const std::string& response,
      const std::map<std::string, std::string>& headers) override;

 private:
  ledger::LedgerClient* ledger_client_;
  LoadLedgerStateCallback load_ledger_state_callback_;
  LoadPublisherStateCallback load_publisher_state_callback_;
  LoadPublisherListCallback load_publisher_list_callback_;
  SaveLedgerStateCallback save_ledger_state_callback_;
  SavePublisherStateCallback save_publisher_state_callback_;
  SavePublishersListCallback save_publishers_list_callback_;
  LoadURLCallback load_url_callback_;

  DISALLOW_COPY_AND_ASSIGN(LedgerClientMojoProxy);
};

} // namespace bat_ledger

#endif  // BRAVE_COMPONENTS_SERVICES_BAT_LEDGER_PUBLIC_CPP_LEDGER_CLIENT_MOJO_PROXY_H_
