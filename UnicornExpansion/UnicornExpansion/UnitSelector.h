#pragma once

#include <vector>

class UnitSelector
{
private:
	std::vector<int> uids;
public:
	void selectUnit(int uid) ;
	void unSelectUnit(int uid);
	void unSelectAll();
	bool isSelectedOne() const;
	int getSelectedUID() const;
	bool isUnitSelected(int uid) const;
	const std::vector<int>& getSelectedUnits() const;
};

