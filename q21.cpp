#include "questions.h"

#include <assert.h>
#include <vector>
#include <numeric>
#include <iostream>
#include <string>
#include <optional>
#include <unordered_set>
#include <unordered_map>

const std::vector<std::string> num_keypad = {
    "789",
    "456",
    "123",
    " 0A"};
constexpr std::pair<size_t, size_t> num_start = std::make_pair(3, 2);

const std::vector<std::string> dir_keypad = {
    " ^A",
    "<v>"};
constexpr std::pair<size_t, size_t> dir_start = std::make_pair(0, 2);


std::optional<std::pair<size_t, size_t>> get_pos(const std::vector<std::string> &keypad, char c) {
    for (size_t i = 0; i < keypad.size(); ++i) {
        for (size_t j = 0; j < keypad[i].size(); ++j) {
            if (keypad[i][j] == c) {
                return std::make_pair(i, j);
            }
        }
    }

    return {};
}

std::optional<std::pair<size_t, size_t>> move(const std::vector<std::string> &keypad, std::pair<size_t, size_t> pos, char dir) {
    std::pair<size_t, size_t> old_pos = pos;
    switch (dir) {
        case '^':
            if (pos.first > 0) {
                pos.first--;
            }
            break;
        case '>':
            if (pos.second < keypad[0].size() - 1) {
                pos.second++;
            }
            break;
        case 'v':
            if (pos.first < keypad.size() - 1) {
                pos.first++;
            }
            break;
        case '<':
            if (pos.second > 0) {
                pos.second--;
            }
            break;
        default: break;
    }

    if (pos != old_pos) {
        return pos;
    }

    return {};
}

void gen_dirs(const std::vector<std::string> &keypad, char start, char end, std::string &dirs) {
    std::pair<ssize_t, ssize_t> start_pos = get_pos(keypad, start).value();
    std::pair<ssize_t, ssize_t> end_pos = get_pos(keypad, end).value();

    std::pair<ssize_t, ssize_t> diff = std::make_pair(end_pos.first - start_pos.first, end_pos.second - start_pos.second);

    char c = '^';
    if (diff.first > 0) {
        c = 'v'; 
    }
    for (size_t i = 0; i < std::abs(diff.first); ++i) {
        dirs += c;
    }

    c = '<';
    if (diff.second > 0) {
        c = '>';
    }
    for (size_t i = 0; i < std::abs(diff.second); ++i) {
        dirs += c;
    }

    dirs += 'A';
}

bool valid_sequence(const std::vector<std::string> &keypad, std::string &sequence, std::pair<size_t, size_t> pos) {
    if (keypad[pos.first][pos.second] == ' ') {
        return false;
    }

    for (char c : sequence) {
        std::optional<std::pair<size_t, size_t>> potential_pos = move(keypad, pos, c);    
        if (!potential_pos.has_value()) {
            return false;
        } else {
            pos = potential_pos.value();
        }
        if (keypad[pos.first][pos.second] == ' ') {
            return false;
        }
    }

    return true;
}

void gen_subseqs(
    const std::vector<std::string> &keypad,
    std::pair<size_t, size_t> pos,
    unsigned int k,
    std::string &subseq,
    std::unordered_set<std::string> &unique_subseqs,
    std::unordered_set<std::string> &valid_subseqs) {
    if (k == 1) {
        if (!unique_subseqs.contains(subseq)) {
            unique_subseqs.emplace(subseq);
            bool is_valid = valid_sequence(keypad, subseq, pos);
            if (is_valid) {
                valid_subseqs.emplace(subseq);
            }
        }
        return;
    }

    gen_subseqs(keypad, pos, k - 1, subseq, unique_subseqs, valid_subseqs);
    for (unsigned int i = 0; i < k - 1; ++i) {
        if (k % 2 == 0) {
            std::swap(subseq[i], subseq[k - 1]); 
        } else {
            std::swap(subseq[0], subseq[k - 1]);
        }
        gen_subseqs(keypad, pos, k - 1, subseq, unique_subseqs, valid_subseqs);
    }
}

std::unordered_map<std::string, std::string> optimal_sequence;
std::unordered_map<std::string, std::string> transitions;

std::string shortest_sequence(
    const std::vector<std::string> &keypad,
    const unsigned int depth, 
    const std::string &parent_seq,
    std::vector<std::string> &seqs
) {
    std::string combined_seqs = std::accumulate(seqs.begin(), seqs.end(), std::string(), 
        [](const std::string& a, const std::string& b) -> std::string { 
            return a + (a.length() > 0 ? "," : "") + b; 
        } );

    combined_seqs += std::to_string(depth);
    if (optimal_sequence.contains(combined_seqs)) {
        // return optimal_sequence[combined_seqs];
    }

    std::string shortest_seq = "";

    std::pair<size_t, size_t> pos = get_pos(keypad, 'A').value();
    char prev = 'A';
    for (size_t i = 0; i < seqs.size(); ++i) {
        // Remove 'A' from end for permutation generation
        std::string seq = seqs[i].substr(0, seqs[i].size() - 1);

        std::cout << parent_seq[i] << " " << seqs[i] << " " << depth << " " << i << " " << parent_seq << std::endl;
        std::pair<size_t, size_t> next_pos = get_pos(keypad, parent_seq[i]).value();
        std::string transition_key = std::string{prev} + std::string{parent_seq[i]} + std::to_string(depth);
        prev = parent_seq[i];

        if (transitions.contains(transition_key)) {
            shortest_seq += transitions[transition_key]; 
            continue;
        }

        std::unordered_set<std::string> valid_seqs;
        std::unordered_set<std::string> unique_seqs;
        if (seq.size() > 0) {
            gen_subseqs(keypad, pos, seq.size(), seq, unique_seqs, valid_seqs);
        } else {
            valid_seqs.insert(seq);
        }

        std::string shortest_subseq = "";

        for (const std::string &valid_seq : valid_seqs) {
            std::string v_seq = valid_seq + 'A';

            if (depth == 0) {
                shortest_subseq = v_seq;
                break;
            }

            std::vector<std::string> subseqs;
            char c = 'A';
            for (char dir : v_seq) {
                std::string dirs;
                gen_dirs(dir_keypad, c, dir, dirs);
                subseqs.emplace_back(dirs);
                c = dir;
            }

            std::string shortest_subseq_candidate = shortest_sequence(dir_keypad, depth - 1, v_seq, subseqs);

            if (shortest_subseq == "" || shortest_subseq_candidate.size() < shortest_subseq.size()) {
                shortest_subseq = shortest_subseq_candidate;
            }
        }

        transitions[transition_key] = shortest_subseq;
        shortest_seq += shortest_subseq;
        pos = next_pos;
    }

    optimal_sequence[combined_seqs] = shortest_seq;
    std::cout << combined_seqs << " " << shortest_seq << std::endl;
    return shortest_seq;
}

void q21(std::ifstream &input_file) {
    std::string line;
    unsigned int total = 0;
    while (std::getline(input_file, line)) {
        char c = 'A';
        std::vector<std::string> seq_dirs;
        for (char dir : line) {
            std::string dirs;
            gen_dirs(num_keypad, c, dir, dirs);
            seq_dirs.emplace_back(dirs);
            c = dir;
        }

        std::string result = shortest_sequence(num_keypad, 2, line, seq_dirs);
        std::cout << result.size() << " " << result << std::endl;
        unsigned long long score = std::stoi(line.substr(0, line.size() - 1)) * result.size();
        std::cout << score << std::endl;
        total += score;
    }
    std::cout << total << std::endl;
}
