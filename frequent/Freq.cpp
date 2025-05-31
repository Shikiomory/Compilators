#include "Freq.h"

void Freq::put(Lex lex) {
	freqdict[lex]++;
	count++;
}

void Freq::print() {
	std::vector<std::pair<Lex, int>> sortedVec(freqdict.begin(), freqdict.end());
	std::sort(sortedVec.begin(), sortedVec.end(), [this](const auto& a, const auto& b) {return lexToStr[a.first] < lexToStr[b.first];});
	int i = 1;
	float perc;
	std::cout << std::endl << std::endl;
	std::cout << std::left << std::setw(5) << "№" << std::setw(20) << "Лексема" << std::setw(12) << "Количество"  << "% " << std::endl;
	for (const auto& it : sortedVec) {
		perc = 100 * static_cast<float>(it.second) / count;
		std::cout << std::left << std::setw(5) << i << std::setw(20) << lexToStr[it.first]  << std::setw(12) << it.second  << perc << " %" << std::endl;
		i++;
	}
	std::cout << std::endl << "Общее число лексем: " << count << std::endl;
}