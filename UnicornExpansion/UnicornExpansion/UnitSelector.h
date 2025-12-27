#pragma once

#include <vector>

class UnitSelector
{
private:
	std::vector<int> uids;
public:
	void selectOneUnit(int uid);
	void invertUnit(int uid);
	void unSelectUnit(int uid);
	void unSelectAll();
	bool isNoSelected() const;
	bool isSelectedOne() const;
	bool isSelectedMulti() const;
	int getSelectedUID() const;
	bool isUnitSelected(int uid) const;
	const std::vector<int>& getSelectedUnits() const;
};

