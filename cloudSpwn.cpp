#include "cloudSpwn.h"
#include "cloud.h"

CloudSpwn::CloudSpwn(int delay) {
	this -> delay = delay;
	elapsed = 0;
}

void CloudSpwn::update() {
	elapsed++;
	if(elapsed >= delay) {
		elapsed = 0;
		Cloud::clouds.push_back(new Cloud());
	}
}