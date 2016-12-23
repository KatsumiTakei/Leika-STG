#pragma once

class CircleShape;
class RectShape;

bool CircleToCircle(const CircleShape& s1, const CircleShape& s2);
bool CircleToRect(const CircleShape& s1, const RectShape& s2);
bool RectToRect(const RectShape& s1, const RectShape& s2);
