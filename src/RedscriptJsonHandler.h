#include "Poco/JSON/Handler.h"
#include "Poco/JSON/JSON.h"
#include "Poco/JSONString.h"
#include <RED4ext/RED4ext.hpp>

using namespace std;
using namespace Poco::JSON;
using namespace Poco;
using namespace RED4ext;

#pragma once

class RedscriptJsonHandler : public Handler
{
public:
  using Ptr = Poco::SharedPtr<RedscriptJsonHandler>;

  RedscriptJsonHandler(Handle<IScriptable> schema);
  /// Creates the RedscriptJsonHandler.

  ~RedscriptJsonHandler();
  /// Destroys the RedscriptJsonHandler.

  template <typename T> void setProp(const string typeName, T value);
  /// Sets property on the Redscript class.

  void reset();
  /// Resets the handler state.

  void startObject();
  /// The parser has read a '{'; a new object is started.
  /// If indent is greater than zero, a newline will be appended.

  void endObject();
  /// The parser has read a '}'; the object is closed.

  void startArray();
  /// The parser has read a [; a new array will be started.
  /// If indent is greater than zero, a newline will be appended.

  void endArray();
  /// The parser has read a ]; the array is closed.

  void key(const string& k);
  /// A key of an object is read; it will be written to the output,
  /// followed by a ':'. If indent is greater than zero, the colon
  /// is padded by a space before and after.

  void null();
  /// A null value is read; "null" will be written to the output.

  void value(int v);
  /// An integer value is read.

  void value(unsigned v);
  /// An unsigned value is read. This will only be triggered if the
  /// value cannot fit into a signed int.

#if defined(POCO_HAVE_INT64)
  void value(Int64 v);
  /// A 64-bit integer value is read; it will be written to the output.

  void value(UInt64 v);
  /// An unsigned 64-bit integer value is read; it will be written to the
  /// output.
#endif

  void value(const string& value);
  /// A string value is read; it will be formatted and written to the output.

  void value(double d);
  /// A double value is read; it will be written to the output.

  void value(bool b);
  /// A boolean value is read; it will be written to the output.

  void comma();
  /// A comma is read; it will be written to the output as "true" or "false".

private:
  bool isInArray() const;
  bool isInObject() const;
  bool isInRoot() const;
  template <typename T> DynArray<T> currentArray();
  void setIntValue(string rsType, CProperty* prop, Int64 v);

  Handle<IScriptable> _schema;

  string _currentKey;
  ScriptInstance _scriptInstance;
  CClass* _instanceType;
  int _array;
  bool _objStart;
  vector<CProperty*> _insideArrays;
  vector<CProperty*> _insideObjects;
  vector<CProperty*> _insideObjects2;
};

inline bool RedscriptJsonHandler::isInArray() const { return _insideArrays.size() > 0; }
inline bool RedscriptJsonHandler::isInObject() const { return _insideObjects.size() > 1; }
inline bool RedscriptJsonHandler::isInRoot() const { return _insideObjects.size() == 1; }
