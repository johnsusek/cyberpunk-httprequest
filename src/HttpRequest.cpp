#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <RED4ext/RED4ext.hpp>
#include <iostream>

namespace HttpRequest {
using namespace std;
using namespace RED4ext;
using namespace Poco::JSON;

struct HttpRequest : IScriptable {
  CClass *GetNativeType();
};

TTypedClass<HttpRequest> httpRequest("HttpRequest");

CClass *HttpRequest::GetNativeType() { return &httpRequest; }

void parseJson() {
  std::string json = "{ \"test\" : { \"property\" : \"value\" } }";
  Parser parser;
  auto result = parser.parse(json);

  // use pointers to avoid copying
  Object::Ptr object = result.extract<Object::Ptr>();
  auto test = object->get("test"); // holds { "property" : "value" }

  Object::Ptr subObject = test.extract<Object::Ptr>();
  test = subObject->get("property");
  std::string val = test.toString(); // val holds "value"

  // copy/convert to Poco::DynamicStruct
  Poco::DynamicStruct ds = *object;
  auto val2 = ds["test"]["property"]; // val holds "value"

  cout << val << endl;
  cout << val2.toString() << endl;
}

void open(IScriptable *aContext, CStackFrame *aFrame, CString *aOut,
          int64_t a4) {
  RED4EXT_UNUSED_PARAMETER(aContext);
  RED4EXT_UNUSED_PARAMETER(a4);

  CString method;
  GetParameter(aFrame, &method);
  CString url;
  GetParameter(aFrame, &url);
  Handle<IScriptable> handler;
  GetParameter(aFrame, &handler);
  Handle<IScriptable> schema;
  GetParameter(aFrame, &schema);

  parseJson();

  // auto instance = schema.instance;
  // auto instanceType = instance->GetType();

  // RED4ext::DynArray<RED4ext::CProperty *> props;
  // instanceType->GetProperties(props);

  // for (auto &prop : props) {
  //   auto name = prop->name.ToString();
  //   CName propType = prop->type->GetName();
  //   auto type = propType.ToString();
  // Get key with `name` from JSON and check if it is `type`
  // then SetValue on the prop
  // }

  aFrame->code++;

  // auto onWrite = [handler](string data, intptr_t userdata) -> bool {
  //   handler.instance->ExecuteFunction(CName("OnWrite"),
  //   CString(data.data())); return true;
  // };

  // auto onHeader = [handler](string data, intptr_t userdata) -> bool {
  //   handler.instance->ExecuteFunction(CName("OnHeader"),
  //   CString(data.data())); return true;
  // };

  // auto onProgress = [handler](cpr_off_t downloadTotal, cpr_off_t downloadNow,
  //                             cpr_off_t uploadTotal, cpr_off_t uploadNow,
  //                             intptr_t userdata) -> bool {
  //   handler.instance->ExecuteFunction(CName("OnProgress"), downloadTotal,
  //                                     downloadNow, uploadTotal, uploadNow);
  //   return true;
  // };

  // if (method == "GET") {
  //   std::ofstream of("1.jpg", std::ios::binary);
  //   cpr::Response r =
  //       cpr::Download(of, cpr::Url{"http://www.httpbin.org/1.jpg"});

  //   AsyncResponse fr =
  //       GetAsync(Url{url.c_str()}, WriteCallback(onWrite),
  //                HeaderCallback(onHeader), ProgressCallback(onProgress));
  // }
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

} // namespace HttpRequest
