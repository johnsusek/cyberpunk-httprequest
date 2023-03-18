# HttpRequest

Make HTTP requests inside Cyberpunk 2077 mods.

<img width="100%" alt="httprequest" src="https://user-images.githubusercontent.com/611996/226085826-bd5f537c-cad3-494b-90f6-4cbbda51be68.png">

## Installation

Unzip the dlls into `red4ext\plugins\httprequest`

## Usage

```
public native class HttpRequest extends IScriptable {
  public native func open(method: String, url: String, handler: ref<IScriptable>) -> String;
}

public class ExampleSystem extends ScriptableSystem {
  private func OnAttach() -> Void {
    let c = new HttpRequest();
    c.open("GET", "https://httpbin.org/anything?foo=bar", this);
  }

  public func OnWrite(data: String) -> Void {
    Log("ExampleSystem::OnWrite" + data);
  }

  public func OnHeader(data: String) -> Void {
    Log("ExampleSystem::OnHeader " + data);
  }

  public func OnProgress(downloadTotal: Int64, downloadNow: Int64, uploadTotal: Int64, uploadNow: Int64) -> Void {
    Log(s"ExampleSystem::OnProgress downloadTotal: \(downloadTotal) downloadNow: \(downloadNow) uploadTotal: \(uploadTotal) uploadNow: \(uploadNow)");
  }
}
```

## Details

Your redscript handler class should implement at least one of these functions:

`public func OnWrite(data: String) -> Void`

This callback function gets called by libcurl as soon as there is data
received that needs to be saved, and provides for streaming downloads. You
could buffer data in your own way, or write every chunk immediately out to
some other stream or file.

`public func OnHeader(data: String) -> Void`

This callback function gets called by libcurl once for each non-data line
received from the server. This includes empty lines and the HTTP status line.
\r\n endings are preserved.

`public func OnProgress(downloadTotal: Int64, downloadNow: Int64, uploadTotal: Int64, uploadNow: Int64) -> Void`

While data is being transferred it will be called very frequently, and during
slow periods like when nothing is being transferred it can slow down to about
one call per second. The callback gets told how much data libcurl will
transfer and has transferred, in number of bytes.
Return true to continue the transfer, and false to cancel it.
