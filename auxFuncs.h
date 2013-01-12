#pragma once

class WLayout;

double RadToDeg(double rad);
double DegToRad(double deg);

bool isNumberChar(int key);
bool isLetterChar(int key);

void checkErrorDebug(CString errorMsg = "");

void checkReleaseError(CString errorMsg);

vector<CString> getFileNames(CString dir);

int parseInt(CString str);

void writeStringToFile(FILE* file, CString str);

void writeToFile(void* buffer, size_t size, int count, FILE* file);

bool readFromFile(void* buffer, size_t size, int count, FILE* file);

CString readStringFromFile(FILE* file);