/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_rewards/utility/ledger_service.h"

#include <utility>

#include "brave/components/brave_rewards/utility/ledger_impl.h"
#include "mojo/public/cpp/bindings/strong_binding.h"

namespace {

void OnLedgerRequest(
    service_manager::ServiceContextRefFactory* ref_factory,
    rewards::mojom::LedgerRequest request) {
  mojo::MakeStrongBinding(
      std::make_unique<rewards::LedgerImpl>(ref_factory->CreateRef()),
      std::move(request));
}

} // namespace

namespace rewards {

LedgerService::LedgerService() {}

LedgerService::~LedgerService() {}

std::unique_ptr<service_manager::Service>
LedgerService::CreateService() {
  return std::make_unique<LedgerService>();
}

void LedgerService::OnStart() {
  ref_factory_.reset(new service_manager::ServiceContextRefFactory(
      context()->CreateQuitClosure()));
  registry_.AddInterface(
      base::Bind(&OnLedgerRequest, ref_factory_.get()));
}

void LedgerService::OnBindInterface(
    const service_manager::BindSourceInfo& source_info,
    const std::string& interface_name,
    mojo::ScopedMessagePipeHandle interface_pipe) {
  registry_.BindInterface(interface_name, std::move(interface_pipe));
}

} // namespace rewards
