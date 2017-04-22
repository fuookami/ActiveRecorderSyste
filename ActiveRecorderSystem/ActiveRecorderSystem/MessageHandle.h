#pragma once

#include "Basic.h"
#include <sstream>
#include <string>
#include <QtWidgets/QMessageBox>

void handleErrorHR(const char *str, HRESULT hr);
void handleErrorHR(const std::string &str, HRESULT hr);
void handleError(const char *str);
void handleError(const std::string &str);
void handleProcess(const char *str);
void handleProcess(const std::string &str);