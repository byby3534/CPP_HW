#pragma once

class Phone
{
public:
	virtual void displayBrand() = 0;
	virtual void showFeature() = 0;
	virtual ~Phone() = default;
};