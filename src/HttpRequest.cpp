#include <RED4ext/RED4ext.hpp>
#include <cpr/cpr.h>

namespace HttpRequest {
using namespace cpr;
using namespace RED4ext;

struct HttpRequest : IScriptable {
  CClass *GetNativeType();
};

TTypedClass<HttpRequest> httpRequest("HttpRequest");

CClass *HttpRequest::GetNativeType() { return &httpRequest; }

void open(IScriptable *aContext, CStackFrame *aFrame, CString *aOut, int64_t a4) {
  RED4EXT_UNUSED_PARAMETER(aContext);
  RED4EXT_UNUSED_PARAMETER(a4);

  CString method;
  GetParameter(aFrame, &method);
  CString url;
  GetParameter(aFrame, &url);
  Handle<IScriptable> handler;
  GetParameter(aFrame, &handler);

  aFrame->code++;

  auto onWrite = [handler](std::string data, intptr_t userdata) -> bool {
    handler.instance->ExecuteFunction(CName("OnWrite"), CString(data.data()));
    return true;
  };

  auto onHeader = [handler](std::string data, intptr_t userdata) -> bool {
    handler.instance->ExecuteFunction(CName("OnHeader"), CString(data.data()));
    return true;
  };

  auto onProgress = [handler](cpr_off_t downloadTotal, cpr_off_t downloadNow, cpr_off_t uploadTotal, cpr_off_t uploadNow, intptr_t userdata) -> bool {
    handler.instance->ExecuteFunction(CName("OnProgress"), downloadTotal, downloadNow, uploadTotal, uploadNow);
    return true;
  };

  if (method == "GET") {
    AsyncResponse fr = GetAsync(
        Url{url.c_str()},
        WriteCallback(onWrite),
        HeaderCallback(onHeader),
        ProgressCallback(onProgress));
  }
}

void RegisterTypes() {
  CNamePool::Add("HttpRequest");
  httpRequest.flags = {.isNative = true};
  CRTTISystem::Get()->RegisterType(&httpRequest);
}

void PostRegisterTypes() {
  auto rtti = CRTTISystem::Get();
  auto scriptable = rtti->GetClass("IScriptable");
  httpRequest.parent = scriptable;

  auto openFn = CClassFunction::Create(&httpRequest, "open", "open", &open);
  openFn->flags = {.isNative = true};
  openFn->SetReturnType("String");
  httpRequest.RegisterFunction(openFn);
}

} // namespace http
