public native class HttpRequest extends IScriptable {
  public native func open(method: String, url: String, handler: ref<IScriptable>) -> String;
}
