#include "UnitSelector.h"

void UnitSelector::selectOneUnit(int uid) {
	uids.clear();
	uids.push_back(uid);
}

void UnitSelector::invertUnit(int uid) {
	if (isUnitSelected(uid)) unSelectUnit(uid); else uids.push_back(uid);
}

void UnitSelector::unSelectUnit(int uid) {
	for (int i=0; i<uids.size(); i++)
		if (uids[i] == uid) {
			uids.erase(uids.begin() + i);
			return;
		}
}

void UnitSelector::unSelectAll() {
	uids.clear();
}

bool UnitSelector::isNoSelected() const {
	return uids.size() == 0;
}

bool UnitSelector::isSelectedOne() const {
	return uids.size() == 1;
}

bool UnitSelector::isSelectedMulti() const {
	return uids.size() > 1;
}

int UnitSelector::getSelectedUID() const {
	if (uids.size() == 0) return -1;
	return uids[0];
}

bool UnitSelector::isUnitSelected(int uid) const {
	for (int id : uids)
		if (id == uid) return true;
	return false;
}

const std::vector<int>& UnitSelector::getSelectedUnits() const {
	return uids;
}