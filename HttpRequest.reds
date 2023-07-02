
public native class HttpRequest extends IScriptable {
	public native func open(method: String, url: String, handler: ref<IScriptable>) -> Bool;
}

public class ResponseAddress extends IScriptable {
  let address: Int32;
  let street: String;
}

public class ResponseMeta extends IScriptable {
  let name: String;
  let pop: Int32;
  let rate: Float;
  let nested: ref<ResponseAddress>;
}

public class ResponseSchema extends IScriptable {
  let string: String;
  let numberInt: Int32;
  let numberUInt: Uint32;
  let numberInt64: Int64;
  let numberUInt64: Uint64;
  let numberFloat: Float;
  let numberDouble: Double;
  let numberFloatBig: Float;
  let numberDoubleBig: Double;
  let boolean: Bool;
  let strings: array<String>;
  let bools: array<Bool>;
  let ints: array<Int32>;
  let ints64: array<Int64>;
  let floats: array<Float>;
  let doubles: array<Double>;
  let variants: array<Variant>;
  let meta: ref<ResponseMeta>;
}

public class LevelSchema extends IScriptable {
  let label: String;
  let level: ref<LevelSchema>;
}

public class ExampleSystem extends ScriptableSystem {
 	public func OnAttach() -> Void {
		let c = new HttpRequest();
		let s = new LevelSchema();
    s.level = new LevelSchema();

		c.open("GET", "http://2077.johnsolo.net/sample.json", s);

		Log("LevelSchema: s.label " + ToString(s.label));
 }
}
