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
