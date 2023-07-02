//
// RedscriptJsonHandler.cpp
//
// Library: JSON
// Package: JSON
// Module:  RedscriptJsonHandler
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "RedscriptJsonHandler.h"
#include <Poco/JSON/Handler.h>
#include <Poco/JSON/Stringifier.h>
#include <iostream>

using namespace Poco::JSON;
using namespace std;

RedscriptJsonHandler::RedscriptJsonHandler(Handle<IScriptable> schema)
    : _schema(schema), _array(0), _objStart(true)
{
  _scriptInstance = static_cast<ScriptInstance>(_schema.instance);
  _instanceType = _schema.instance->GetType();
}

RedscriptJsonHandler::~RedscriptJsonHandler() {}

void RedscriptJsonHandler::reset()
{
  _array = 0;
  _objStart = true;
}

void RedscriptJsonHandler::startObject()
{
  CProperty* prop = NULL;

  if (isInObject()) {
    CProperty* containerProp = _insideObjects.back();
    auto containerPropValue = containerProp->GetValue<Handle<IScriptable>>(_scriptInstance);
    prop = containerPropValue.instance->GetType()->GetProperty(_currentKey.c_str());
  }
  else {
    prop = _instanceType->GetProperty(_currentKey.c_str());
  }

  if (!prop) {
    cerr << "[RedscriptJsonHandler] Couldn't find object prop named " << _currentKey << " in RS class"
         << endl;

    // This allows object walking to continue - will just skip setting RS props
    _insideObjects.emplace_back(nullptr);
  }
  else {
    // string propType = prop->type->GetName().ToString();
    _insideObjects.emplace_back(prop);
  }

  _objStart = true;
}

void RedscriptJsonHandler::endObject()
{
  _insideObjects.pop_back();
  _objStart = false;
}

void RedscriptJsonHandler::startArray()
{
  ++_array;
  _objStart = true;

  CProperty* prop = _instanceType->GetProperty(_currentKey.c_str());

  if (!prop) {
    cerr << "[RedscriptJsonHandler] Couldn't find array prop named " << _currentKey << " in RS class"
         << endl;

    // This allows array walking to continue - will just skip setting RS props
    _insideArrays.emplace_back(nullptr);
  }
  else {
    _insideArrays.emplace_back(prop);
  }
}

void RedscriptJsonHandler::endArray()
{
  --_array;
  poco_assert(_array >= 0);
  _objStart = false;
  _insideArrays.pop_back();
}

/// @brief JSON key
/// @param k
void RedscriptJsonHandler::key(const std::string& k)
{
  _currentKey = k;

  if (!_objStart) comma();

  _objStart = true;
}

/// @brief Null JSON values
void RedscriptJsonHandler::null()
{
  CProperty* prop = _instanceType->GetProperty(_currentKey.c_str());

  if (!isInArray() && !isInObject()) {
    _currentKey = "";
  }

  if (!prop) return;

  string propType = prop->type->GetName().ToString();

  if (isInArray()) {
    CProperty* currentArrayProp = _insideArrays.back();
    string currentArrayType = currentArrayProp->type->GetName().ToString();

    if (currentArrayType == "array:Variant") {
      auto propValue = prop->GetValue<DynArray<Variant>>(_scriptInstance);

      Variant out;
      propValue.EmplaceBack(out);

      prop->SetValue(_scriptInstance, propValue);
    }
  }
  else {
    prop->SetValue(_scriptInstance, nullptr);
  }

  _objStart = false;
}

void RedscriptJsonHandler::setIntValue(string propType, CProperty* prop, Int64 v)
{
  if (propType == "Int64") {
    prop->SetValue(_scriptInstance, v);
  }
  else if (propType == "Uint64") {
    prop->SetValue(_scriptInstance, uint64_t(v));
  }
  else if (propType == "Int32") {
    prop->SetValue(_scriptInstance, int32_t(v));
  }
  else if (propType == "Uint32") {
    prop->SetValue(_scriptInstance, uint32_t(v));
  }
  else if (propType == "Int16") {
    prop->SetValue(_scriptInstance, int16_t(v));
  }
  else if (propType == "Uint16") {
    prop->SetValue(_scriptInstance, uint16_t(v));
  }
  else if (propType == "Int8") {
    prop->SetValue(_scriptInstance, int8_t(v));
  }
  else if (propType == "Uint8") {
    prop->SetValue(_scriptInstance, uint8_t(v));
  }
  else {
    cerr << "[RedscriptJsonHandler] Don't know how to cast JSON Integer to RS type " << propType
         << " for prop " << _currentKey << endl;
  }
}

#if defined(POCO_HAVE_INT64)
/// @brief 64-bit int
void RedscriptJsonHandler::value(Int64 v)
{
  if (!isInArray() && !isInObject()) {
    _currentKey = "";
  }

  if (isInArray()) {
    CProperty* prop = _instanceType->GetProperty(_currentKey.c_str());
    if (!prop) return;

    string propType = prop->type->GetName().ToString();

    CProperty* currentArrayProp = _insideArrays.back();
    string currentArrayType = currentArrayProp->type->GetName().ToString();

    if (currentArrayType == "array:Int64") {
      auto propValue = prop->GetValue<DynArray<int64_t>>(_scriptInstance);

      propValue.EmplaceBack(v);

      prop->SetValue(_scriptInstance, propValue);
    }
    else if (currentArrayType == "array:Uint64") {
      auto propValue = prop->GetValue<DynArray<uint64_t>>(_scriptInstance);

      propValue.EmplaceBack(uint64_t(v));

      prop->SetValue(_scriptInstance, propValue);
    }
    else if (currentArrayType == "array:Int32") {
      auto propValue = prop->GetValue<DynArray<int32_t>>(_scriptInstance);

      propValue.EmplaceBack(int32_t(v));

      prop->SetValue(_scriptInstance, propValue);
    }
    else if (currentArrayType == "array:Uint32") {
      auto propValue = prop->GetValue<DynArray<uint32_t>>(_scriptInstance);

      propValue.EmplaceBack(uint32_t(v));

      prop->SetValue(_scriptInstance, propValue);
    }
    else if (currentArrayType == "array:Int16") {
      auto propValue = prop->GetValue<DynArray<int16_t>>(_scriptInstance);

      propValue.EmplaceBack(int16_t(v));

      prop->SetValue(_scriptInstance, propValue);
    }
    else if (currentArrayType == "array:Uint16") {
      auto propValue = prop->GetValue<DynArray<uint16_t>>(_scriptInstance);

      propValue.EmplaceBack(uint16_t(v));

      prop->SetValue(_scriptInstance, propValue);
    }
    else if (currentArrayType == "array:Int8") {
      auto propValue = prop->GetValue<DynArray<int8_t>>(_scriptInstance);

      propValue.EmplaceBack(int8_t(v));

      prop->SetValue(_scriptInstance, propValue);
    }
    else if (currentArrayType == "array:Uint8") {
      auto propValue = prop->GetValue<DynArray<uint8_t>>(_scriptInstance);

      propValue.EmplaceBack(uint8_t(v));

      prop->SetValue(_scriptInstance, propValue);
    }
    else if (currentArrayType == "array:Variant") {
      auto propValue = prop->GetValue<DynArray<Variant>>(_scriptInstance);

      auto propTypeClass = RED4ext::CRTTISystem::Get()->GetType(CName(propType.c_str()));
      Variant out;
      out.Fill(propTypeClass, &v);
      propValue.EmplaceBack(out);

      prop->SetValue(_scriptInstance, propValue);
    }
    else {
      cerr << "[RedscriptJsonHandler] Don't know how to insert JSON Integer for RS array " << propType
           << " for prop " << _currentKey << endl;
    }
  }
  else if (isInObject()) {
    CProperty* containerProp = _insideObjects.back();
    if (!containerProp) return;

    auto containerPropValue = containerProp->GetValue<Handle<IScriptable>>(_scriptInstance);
    auto prop = containerPropValue.instance->GetType()->GetProperty(_currentKey.c_str());

    if (!prop) return;

    string propType = prop->type->GetName().ToString();
    setIntValue(propType, prop, v);

    containerProp->SetValue(_scriptInstance, containerPropValue);
  }
  else {
    CProperty* prop = _instanceType->GetProperty(_currentKey.c_str());
    if (!prop) return;

    string propType = prop->type->GetName().ToString();
    setIntValue(propType, prop, v);
  }

  _objStart = false;
}

/// @brief 64-bit unsigned int
void RedscriptJsonHandler::value(UInt64 v)
{
  if (!isInArray() && !isInObject()) {
    _currentKey = "";
  }

  _objStart = false;
}
#endif

/// @brief String
void RedscriptJsonHandler::value(const std::string& value)
{
  if (!isInArray() && !isInObject()) {
    _currentKey = "";
  }

  if (isInArray()) {
    CProperty* prop = _instanceType->GetProperty(_currentKey.c_str());

    if (!prop) return;

    CProperty* currentArrayProp = _insideArrays.back();
    string currentArrayType = currentArrayProp->type->GetName().ToString();

    if (currentArrayType == "array:String") {
      CString stringValue(value.c_str());
      auto propValue = prop->GetValue<DynArray<CString>>(_scriptInstance);
      propValue.EmplaceBack(stringValue);
      prop->SetValue(_scriptInstance, propValue);
    }
    else if (currentArrayType == "array:Variant") {
      CString stringValue(value.c_str());
      auto propValue = prop->GetValue<DynArray<Variant>>(_scriptInstance);

      auto stringType = RED4ext::CRTTISystem::Get()->GetType("String");
      Variant out;
      out.Fill(stringType, &stringValue);

      propValue.EmplaceBack(out);

      prop->SetValue(_scriptInstance, propValue);
    }
    else {
      cerr << "[RedscriptJsonHandler] Could not insert JSON String into array of type "
           << currentArrayType << " when trying to set key " << _currentKey << endl;
    }
  }
  else if (isInObject()) {
    // container that holds the prop for the string value we are about to set
    CProperty* containerProp = _insideObjects.back();

    if (!containerProp) return; // TODO error

    // _scriptInstance is what we use to get/set this container object (if in root)
    // (if we aren't in root we get/set on the parent of containerProp)
    // containerPropValue.instance is what we use to get/set this string value

    if (isInRoot()) {
      auto containerPropValue = containerProp->GetValue<Handle<IScriptable>>(_scriptInstance);
      auto prop = containerPropValue.instance->GetType()->GetProperty(_currentKey.c_str());
      if (!prop) return;
      string propType = prop->type->GetName().ToString();
      if (propType == "String") {
        CString stringValue(value.c_str());
        prop->SetValue(static_cast<ScriptInstance>(containerPropValue.instance), stringValue);
      }
      else {
        cerr << "[RedscriptJsonHandler] JSON is a String but object prop is a " << propType
             << " when trying to set key " << _currentKey << endl;
      }
      containerProp->SetValue(_scriptInstance, containerPropValue);
    }
    else {
      // Nested Data
    }
  }
  else {
    CProperty* prop = _instanceType->GetProperty(_currentKey.c_str());

    if (!prop) return;

    string propType = prop->type->GetName().ToString();

    if (propType == "String") {
      CString stringValue(value.c_str());
      prop->SetValue(_scriptInstance, stringValue);
    }
    else {
      cerr << "[RedscriptJsonHandler] JSON is a String but object prop is a " << propType
           << " when trying to set key " << _currentKey << endl;
    }
  }

  _objStart = false;
}

/// @brief Float/Double
void RedscriptJsonHandler::value(double d)
{
  if (!isInArray() && !isInObject()) {
    _currentKey = "";
  }

  if (isInArray()) {
    CProperty* prop = _instanceType->GetProperty(_currentKey.c_str());
    if (!prop) return;

    string propType = prop->type->GetName().ToString();

    CProperty* currentArrayProp = _insideArrays.back();
    string currentArrayType = currentArrayProp->type->GetName().ToString();

    if (currentArrayType == "array:Double") {
      auto propValue = prop->GetValue<DynArray<double>>(_scriptInstance);

      propValue.EmplaceBack(d);

      prop->SetValue(_scriptInstance, propValue);
    }
    else if (currentArrayType == "array:Float") {
      auto propValue = prop->GetValue<DynArray<float_t>>(_scriptInstance);

      propValue.EmplaceBack(float_t(d));

      prop->SetValue(_scriptInstance, propValue);
    }
    else if (currentArrayType == "array:Variant") {
      auto propValue = prop->GetValue<DynArray<Variant>>(_scriptInstance);

      auto doubleType = RED4ext::CRTTISystem::Get()->GetType("Double");
      Variant out;
      out.Fill(doubleType, &d);
      propValue.EmplaceBack(out);

      prop->SetValue(_scriptInstance, propValue);
    }
    else {
      cerr << "[RedscriptJsonHandler] Could not insert JSON Double or Float into array of type "
           << currentArrayType << " on prop " << _currentKey << endl;
    }
  }
  else if (isInObject()) {
    CProperty* containerProp = _insideObjects.back();
    if (!containerProp) return;

    auto containerPropValue = containerProp->GetValue<Handle<IScriptable>>(_scriptInstance);
    auto prop = containerPropValue.instance->GetType()->GetProperty(_currentKey.c_str());

    if (!prop) return;

    string propType = prop->type->GetName().ToString();

    if (propType == "Double") {
      prop->SetValue(static_cast<ScriptInstance>(containerPropValue.instance), d);
    }
    else if (propType == "Float") {
      prop->SetValue(static_cast<ScriptInstance>(containerPropValue.instance), float_t(d));
    }
    else {
      cerr << "[RedscriptJsonHandler] Could not use JSON Double or Float as value for type of "
           << propType << endl;
    }

    containerProp->SetValue(_scriptInstance, containerPropValue);
  }
  else {
    CProperty* prop = _instanceType->GetProperty(_currentKey.c_str());
    if (!prop) return;

    string propType = prop->type->GetName().ToString();

    if (propType == "Double") {
      prop->SetValue(_scriptInstance, d);
    }
    else if (propType == "Float") {
      prop->SetValue(_scriptInstance, float_t(d));
    }
    else {
      cerr << "[RedscriptJsonHandler] Could not set JSON Double or Float as value for type of "
           << propType << " on prop " << _currentKey << endl;
    }
  }

  _objStart = false;
}

/// @brief Boolean
void RedscriptJsonHandler::value(bool b)
{
  CProperty* prop = _instanceType->GetProperty(_currentKey.c_str());
  if (!isInArray() && !isInObject()) {
    _currentKey = "";
  }

  if (!prop) return;

  string propType = prop->type->GetName().ToString();

  CProperty* currentArrayProp = _insideArrays.back();
  string currentArrayType = currentArrayProp->type->GetName().ToString();

  if (isInArray()) {
    if (currentArrayType == "array:Bool") {
      auto propValue = prop->GetValue<DynArray<bool>>(_scriptInstance);

      propValue.EmplaceBack(b);

      prop->SetValue(_scriptInstance, propValue);
    }
    else if (currentArrayType == "array:Variant") {
      auto propValue = prop->GetValue<DynArray<Variant>>(_scriptInstance);

      auto boolType = RED4ext::CRTTISystem::Get()->GetType("Bool");
      Variant out;
      out.Fill(boolType, &b);
      propValue.EmplaceBack(out);

      prop->SetValue(_scriptInstance, propValue);
    }
  }
  else {
    if (propType == "Bool") {
      prop->SetValue(_scriptInstance, b);
    }
  }

  _objStart = false;
}

/// @brief 32-bit int - Unused for 64-bit builds
void RedscriptJsonHandler::value(int v)
{
  if (!isInArray() && !isInObject()) {
    _currentKey = "";
  }

  _objStart = false;
}

/// @brief 32-bit unsigned int - Unused for 64-bit builds
void RedscriptJsonHandler::value(unsigned v)
{
  if (!isInArray() && !isInObject()) {
    _currentKey = "";
  }

  _objStart = false;
}

void RedscriptJsonHandler::comma() {}
