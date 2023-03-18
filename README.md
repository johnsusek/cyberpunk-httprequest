# HttpRequest

Your handler class should implement at least one of these functions:

This callback function gets called by libcurl as soon as there is data
received that needs to be saved, and provides for streaming downloads. You
could buffer data in your own way, or write every chunk immediately out to
some other stream or file.

`public func OnWrite(data: String) -> Void`

This callback function gets called by libcurl once for each non-data line
received from the server. This includes empty lines and the HTTP status line.
\r\n endings are preserved.

`public func OnHeader(data: String) -> Void`

While data is being transferred it will be called very frequently, and during
slow periods like when nothing is being transferred it can slow down to about
one call per second. The callback gets told how much data libcurl will
transfer and has transferred, in number of bytes.
Return true to continue the transfer, and false to cancel it.

`public func OnProgress(downloadTotal: Int64, downloadNow: Int64, uploadTotal: Int64, uploadNow: Int64) -> Void`


Example handler class:

```
public class ExampleSystem extends ScriptableSystem {
  private func OnAttach() -> Void {
    let c = new HttpRequest();
    c.open("GET", "https://httpbin.org/anything?foo=bar", this);
  }

  public func OnWrite(data: String) -> Void {
    Log("ExampleSystem::OnWrite");
    Log(data);
  }

  public func OnHeader(data: String) -> Void {
    Log("ExampleSystem::OnHeader");
    Log(data);
  }

  public func OnProgress(downloadTotal: Int64, downloadNow: Int64, uploadTotal: Int64, uploadNow: Int64) -> Void {
    Log(s"ExampleSystem::OnProgress downloadTotal: \(downloadTotal) downloadNow: \(downloadNow) uploadTotal: \(uploadTotal) uploadNow: \(uploadNow)");
  }
}
```