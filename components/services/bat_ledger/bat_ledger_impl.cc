/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/services/bat_ledger/bat_ledger_impl.h"

#include "brave/components/services/bat_ledger/bat_ledger_client_mojo_proxy.h"

namespace bat_ledger {

namespace {

ledger::PUBLISHER_EXCLUDE ToLedgerPublisherExclude(int32_t exclude) {
  return (ledger::PUBLISHER_EXCLUDE)exclude;
}

ledger::PUBLISHER_MONTH ToLedgerPublisherMonth(int32_t month) {
  return (ledger::PUBLISHER_MONTH)month;
}

ledger::ReportType ToLedgerReportType(int32_t type) {
  return (ledger::ReportType)type;
}

ledger::PUBLISHER_CATEGORY ToLedgerPublisherCategory(int32_t category) {
  return (ledger::PUBLISHER_CATEGORY)category;
}

} // anonymous namespace

BatLedgerImpl::BatLedgerImpl(
    mojom::BatLedgerClientAssociatedPtrInfo client_info)
  : bat_ledger_client_mojo_proxy_(
      new BatLedgerClientMojoProxy(std::move(client_info))),
    ledger_(
      ledger::Ledger::CreateInstance(bat_ledger_client_mojo_proxy_.get())) {
}

BatLedgerImpl::~BatLedgerImpl() {
}

void BatLedgerImpl::Initialize() {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  ledger_->Initialize();
}

void BatLedgerImpl::CreateWallet() {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  ledger_->CreateWallet();
}

void BatLedgerImpl::FetchWalletProperties() {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  ledger_->FetchWalletProperties();
}

void BatLedgerImpl::GetPublisherMinVisitTime(
    GetPublisherMinVisitTimeCallback callback) {
  std::move(callback).Run(ledger_->GetPublisherMinVisitTime());
}

void BatLedgerImpl::GetReconcileStamp(GetReconcileStampCallback callback) {
  std::move(callback).Run(ledger_->GetReconcileStamp());
}

void BatLedgerImpl::OnUnload(uint32_t tab_id, uint64_t current_time) {
  ledger_->OnUnload(tab_id, current_time);
}

void BatLedgerImpl::OnShow(uint32_t tab_id, uint64_t current_time) {
  ledger_->OnShow(tab_id, current_time);
}

void BatLedgerImpl::OnHide(uint32_t tab_id, uint64_t current_time) {
  ledger_->OnHide(tab_id, current_time);
}

void BatLedgerImpl::OnForeground(uint32_t tab_id, uint64_t current_time) {
  ledger_->OnForeground(tab_id, current_time);
}

void BatLedgerImpl::OnBackground(uint32_t tab_id, uint64_t current_time) {
  ledger_->OnBackground(tab_id, current_time);
}

void BatLedgerImpl::OnMediaStart(uint32_t tab_id, uint64_t current_time) {
  ledger_->OnMediaStart(tab_id, current_time);
}

void BatLedgerImpl::OnMediaStop(uint32_t tab_id, uint64_t current_time) {
  ledger_->OnMediaStop(tab_id, current_time);
}

void BatLedgerImpl::SetPublisherExclude(const std::string& publisher_key,
    int32_t exclude) {
  ledger_->SetPublisherExclude(publisher_key,
      ToLedgerPublisherExclude(exclude));
}

void BatLedgerImpl::RestorePublishers() {
  ledger_->RestorePublishers();
}

void BatLedgerImpl::SetBalanceReportItem(int32_t month,
    int32_t year, int32_t type, const std::string& probi,
    SetBalanceReportItemCallback callback) {
  ledger_->SetBalanceReportItem(
      ToLedgerPublisherMonth(month), year, ToLedgerReportType(type), probi);
  std::move(callback).Run();
}

void BatLedgerImpl::OnReconcileCompleteSuccess(const std::string& viewing_id,
    int32_t category, const std::string& probi, int32_t month,
    int32_t year, uint32_t data, OnReconcileCompleteSuccessCallback callback) {
  ledger_->OnReconcileCompleteSuccess(viewing_id,
      ToLedgerPublisherCategory(category), probi,
      ToLedgerPublisherMonth(month), year, data);
  std::move(callback).Run();
}

void BatLedgerImpl::FetchGrant(const std::string& lang,
    const std::string& payment_id) {
  ledger_->FetchGrant(lang, payment_id);
}

void BatLedgerImpl::GetGrantCaptcha() {
  ledger_->GetGrantCaptcha();
}

void BatLedgerImpl::GetWalletPassphrase(GetWalletPassphraseCallback callback) {
  std::move(callback).Run(ledger_->GetWalletPassphrase());
}

void BatLedgerImpl::GetNumExcludedSites(GetNumExcludedSitesCallback callback) {
  std::move(callback).Run(ledger_->GetNumExcludedSites());
}

void BatLedgerImpl::RecoverWallet(const std::string& passPhrase) {
  ledger_->RecoverWallet(passPhrase);
}

void BatLedgerImpl::SolveGrantCaptcha(const std::string& solution) {
  ledger_->SolveGrantCaptcha(solution);
}

void BatLedgerImpl::GetBATAddress(GetBATAddressCallback callback) {
  std::move(callback).Run(ledger_->GetBATAddress());
}

void BatLedgerImpl::GetBTCAddress(GetBTCAddressCallback callback) {
  std::move(callback).Run(ledger_->GetBTCAddress());
}

void BatLedgerImpl::GetETHAddress(GetETHAddressCallback callback) {
  std::move(callback).Run(ledger_->GetETHAddress());
}

void BatLedgerImpl::GetLTCAddress(GetLTCAddressCallback callback) {
  std::move(callback).Run(ledger_->GetLTCAddress());
}

void BatLedgerImpl::SetRewardsMainEnabled(bool enabled) {
  ledger_->SetRewardsMainEnabled(enabled);
}

void BatLedgerImpl::SetPublisherMinVisitTime(uint64_t duration_in_seconds) {
  ledger_->SetPublisherMinVisitTime(duration_in_seconds);
}

void BatLedgerImpl::SetPublisherMinVisits(uint32_t visits) {
  ledger_->SetPublisherMinVisits(visits);
}

void BatLedgerImpl::SetPublisherAllowNonVerified(bool allow) {
  ledger_->SetPublisherAllowNonVerified(allow);
}

void BatLedgerImpl::SetPublisherAllowVideos(bool allow) {
  ledger_->SetPublisherAllowVideos(allow);
}

void BatLedgerImpl::SetUserChangedContribution() {
  ledger_->SetUserChangedContribution();
}

void BatLedgerImpl::SetContributionAmount(double amount) {
  ledger_->SetContributionAmount(amount);
}

void BatLedgerImpl::SetAutoContribute(bool enabled) {
  ledger_->SetAutoContribute(enabled);
}

void BatLedgerImpl::OnTimer(uint32_t timer_id, OnTimerCallback callback) {
  ledger_->OnTimer(timer_id);
  std::move(callback).Run();
}

void BatLedgerImpl::IsWalletCreated(IsWalletCreatedCallback callback) {
  std::move(callback).Run(ledger_->IsWalletCreated());
}

void BatLedgerImpl::GetContributionAmount(
    GetContributionAmountCallback callback) {
  std::move(callback).Run(ledger_->GetContributionAmount());
}

void BatLedgerImpl::RemoveRecurring(const std::string& publisher_key) {
  ledger_->RemoveRecurring(publisher_key);
}

void BatLedgerImpl::SetPublisherPanelExclude(const std::string& publisher_key,
    int32_t exclude, uint64_t window_id) {
  ledger_->SetPublisherPanelExclude(publisher_key,
      ToLedgerPublisherExclude(exclude), window_id);
}

} // namespace bat_ledger
