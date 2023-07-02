#include "Poco/Exception.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "RedscriptJsonHandler.cpp"
#include "RedscriptJsonHandler.h"
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/PrintHandler.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/Net.h>
#include <RED4ext/RED4ext.hpp>
#include <iostream>

using Poco::Exception;
using Poco::Path;
using Poco::StreamCopier;
using Poco::URI;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

namespace HttpRequest {
using namespace std;
using namespace RED4ext;
using namespace Poco::JSON;
using namespace Poco::Net;

struct HttpRequest : IScriptable {
  CClass* GetNativeType();
};

TTypedClass<HttpRequest> httpRequest("HttpRequest");

CClass* HttpRequest::GetNativeType() { return &httpRequest; }

void open(IScriptable* aContext, CStackFrame* aFrame, CString* aOut, int64_t a4)
{
  RED4EXT_UNUSED_PARAMETER(aContext);
  RED4EXT_UNUSED_PARAMETER(a4);

  CString method;
  GetParameter(aFrame, &method);
  CString url;
  GetParameter(aFrame, &url);
  Handle<IScriptable> schema;
  GetParameter(aFrame, &schema);

  auto instanceType = schema.instance->GetType();

  ScriptInstance scriptInstance = static_cast<ScriptInstance>(schema.instance);

  URI uri(url.c_str());
  string path(uri.getPathAndQuery());
  if (path.empty()) path = "/";

  HTTPClientSession session(uri.getHost(), uri.getPort());
  HTTPRequest request(method.c_str(), path, HTTPMessage::HTTP_1_1);
  HTTPResponse response;

  session.sendRequest(request);
  istream& rs = session.receiveResponse(response);
  cout << response.getStatus() << " " << response.getReason() << endl;

  if (response.getStatus() == HTTPResponse::HTTP_OK) {
    RedscriptJsonHandler::Ptr pHandler = new RedscriptJsonHandler(schema);
    Parser parser;
    parser.setHandler(pHandler);
    parser.parse(rs);
    parser.reset();
  }

  aFrame->code++;
}

void RegisterTypes()
{
  CNamePool::Add("HttpRequest");
  httpRequest.flags = {.isNative = true};
  CRTTISystem::Get()->RegisterType(&httpRequest);
}

void PostRegisterTypes()
{
  auto rtti = CRTTISystem::Get();
  auto scriptable = rtti->GetClass("IScriptable");
  httpRequest.parent = scriptable;

  auto openFn = CClassFunction::Create(&httpRequest, "open", "open", &open);
  openFn->flags = {.isNative = true};
  openFn->SetReturnType("String");
  httpRequest.RegisterFunction(openFn);
}

} // namespace HttpRequest
