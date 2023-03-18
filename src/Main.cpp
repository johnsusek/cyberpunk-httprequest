#include <RED4ext/RED4ext.hpp>
#include "HttpRequest.cpp"

RED4EXT_C_EXPORT void RED4EXT_CALL RegisterTypes() {
  HttpRequest::RegisterTypes();
}

RED4EXT_C_EXPORT void RED4EXT_CALL PostRegisterTypes() {
  HttpRequest::PostRegisterTypes();
}

RED4EXT_C_EXPORT bool RED4EXT_CALL Main(RED4ext::PluginHandle aHandle,
                                        RED4ext::EMainReason aReason,
                                        const RED4ext::Sdk *aSdk) {
  RED4EXT_UNUSED_PARAMETER(aHandle);
  RED4EXT_UNUSED_PARAMETER(aSdk);

  switch (aReason) {
  case RED4ext::EMainReason::Load: {
    RED4ext::RTTIRegistrator::Add(RegisterTypes, PostRegisterTypes);
    break;
  }
  case RED4ext::EMainReason::Unload: {
    break;
  }
  }

  return true;
}

RED4EXT_C_EXPORT void RED4EXT_CALL Query(RED4ext::PluginInfo *aInfo) {
  aInfo->name = L"HttpRequest";
  aInfo->author = L"John Susek";
  aInfo->version = RED4EXT_SEMVER(1, 0, 0);
  aInfo->runtime = RED4EXT_RUNTIME_LATEST;
  aInfo->sdk = RED4EXT_SDK_LATEST;
}

RED4EXT_C_EXPORT uint32_t RED4EXT_CALL Supports() {
  return RED4EXT_API_VERSION_LATEST;
}
