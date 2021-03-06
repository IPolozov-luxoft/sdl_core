/*
 * Copyright (c) 2016, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <string>
#include "application_manager/commands/hmi/on_received_policy_update.h"
#include "application_manager/application_manager.h"
#include "utils/file_system.h"

namespace application_manager {

namespace commands {

OnReceivedPolicyUpdate::OnReceivedPolicyUpdate(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : NotificationFromHMI(message, application_manager) {}

OnReceivedPolicyUpdate::~OnReceivedPolicyUpdate() {}

void OnReceivedPolicyUpdate::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
#if defined(PROPRIETARY_MODE) || defined(EXTERNAL_PROPRIETARY_MODE)
  const std::string& file_path =
      (*message_)[strings::msg_params][hmi_notification::policyfile].asString();
  policy::BinaryMessage file_content;
  if (!file_system::ReadBinaryFile(file_path, file_content)) {
    LOG4CXX_ERROR(logger_, "Failed to read Update file.");
    return;
  }
  application_manager_.GetPolicyHandler().ReceiveMessageFromSDK(file_path,
                                                                file_content);
#else
  LOG4CXX_WARN(logger_,
               "This RPC is part of extended policy flow."
               "Please re-build with extended policy mode enabled.");
#endif
}

}  // namespace commands

}  // namespace application_manager
