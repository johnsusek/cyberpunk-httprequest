# HttpRequest

Make HTTP requests inside Cyberpunk 2077 mods.

<img width="100%" alt="httprequest" src="https://user-images.githubusercontent.com/611996/226085826-bd5f537c-cad3-494b-90f6-4cbbda51be68.png">

## Installation

Unzip the dlls into `red4ext\plugins\httprequest`

## Usage

```swift
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

## Callbacks

Your redscript handler class should implement at least one of these functions.

### OnWrite
```swift
public func OnWrite(data: String) -> Void
```

This callback function gets called as soon as there is data
received that needs to be saved, and provides for streaming downloads. You
could buffer data in your own way, or write every chunk immediately out to
some other stream or file.

### OnHeader
```swift
public func OnHeader(data: String) -> Void
```

This callback function gets called once for each non-data line
received from the server. This includes empty lines and the HTTP status line.
\r\n endings are preserved.


### OnProgress
```swift
public func OnProgress(downloadTotal: Int64, downloadNow: Int64, uploadTotal: Int64, uploadNow: Int64) -> Void
```

While data is being transferred it will be called very frequently, and during
slow periods like when nothing is being transferred it can slow down to about
one call per second. The callback gets told how much data libcurl will
transfer and has transferred, in number of bytes.

## Building

```bash
cmake -E make_directory build
cd build
cmake .. -A x64 -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
```
