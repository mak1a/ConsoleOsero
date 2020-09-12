#include "Study.hpp"
#include <thread>
#include <Windows.h>
#include <string>
#include <conio.h>

bnsGame::Study::Study(const InitData& init_)
: IScene(init_)
, m_isChangeScene(false)
, m_parentNum(10)
, m_geneNum(1)
, m_turn(Turn::Player)
, m_aspect(((m_turn == Turn::Player) ? Self : Enemy))
, m_playerIndex(0)
, m_enemyIndex(1)
, m_state(StudyState::Studying) {
    ClearDisplay();

    std::cout << "AIの評価値を学習します。" << std::endl;
    std::cout << "現在のAIの強さを変えますか？(y or n)" << std::endl;
    
    for (std::string s; ; std::cout << "yかnを入力してください。" << std::endl) {
        std::cout << ">>";
        std::cin >> s;
        if (s == "n" || s == "N") {
            m_isChangeScene = true;
            return;
        }
        if (s != "y" || s != "Y") {
            break;
        }
    }

    std::cout << "学習中です。30秒～1分ほど\nお待ちください。";

    for (uint32_t i{}; i < static_cast<uint32_t>(m_parentNum); ++i) {
        m_winNums.emplace_back(static_cast<uint32_t>(0), i);
    }

    m_sikous.emplace_back(true);
    for (uint32_t i{}; i < static_cast<uint32_t>(m_parentNum) - 1; ++i) {
        m_sikous.emplace_back();
    }

    int32_t turn = (m_turn == Turn::Player) ? Self : Enemy;
    if (m_aspect.MakeLegalPuts(turn) <= 0) {
        m_turn = Turn::Result;
    }

    //std::cout << "プレイヤー番号 : " << m_playerIndex << ", 敵番号 : " << m_enemyIndex;
}

void bnsGame::Study::Init(const Turn& turn_) {
    m_turn = turn_;
    m_aspect = Aspect((m_turn == Turn::Player) ? Self : Enemy);
    
    int32_t turn = (m_turn == Turn::Player) ? Self : Enemy;
    if (m_aspect.MakeLegalPuts(turn) <= 0) {
        m_turn = Turn::Result;
    }

    //std::cout << "プレイヤー番号 : " << m_playerIndex << ", 敵番号 : " << m_enemyIndex;
}

void bnsGame::Study::Mating() {
    std::vector<Sikou> children;

    // 優秀な二人の遺伝子はクローンする
    {
        std::sort(m_winNums.begin(), m_winNums.end(), [](auto const& a, auto const& b) {
            return a.first > b.first;
        });
        children.emplace_back(m_sikous[m_winNums[0].second]);
        children.emplace_back(m_sikous[m_winNums[1].second]);
    }

    // 優秀な二人の遺伝子を交配させる
    {
        std::array<std::array<int32_t, 10>, 10> child_1, child_2;

        uint32_t separateX{utl::RandomUint32(1, 8)};
        uint32_t separateY{utl::RandomUint32(1, 8)};

        for (uint32_t y{1}; y < separateY; ++y) {
            for (uint32_t x{1}; x <= 8; ++x) {
                child_1[y][x] = m_sikous[m_winNums[0].second].GetValue(x, y);
                child_2[y][x] = m_sikous[m_winNums[1].second].GetValue(x, y);
            }
        }

        for (uint32_t x{1}; x < separateX; ++x) {
            child_1[separateY][x] = m_sikous[m_winNums[0].second].GetValue(x, separateY);
            child_2[separateY][x] = m_sikous[m_winNums[1].second].GetValue(x, separateY);
        }
        for (uint32_t x{separateX}; x <= 8; ++x) {
            child_1[separateY][x] = m_sikous[m_winNums[1].second].GetValue(x, separateY);
            child_2[separateY][x] = m_sikous[m_winNums[0].second].GetValue(x, separateY);
        }

        for (uint32_t y{separateY + 1}; y <= 8; ++y) {
            for (uint32_t x{1}; x <= 8; ++x) {
                child_1[y][x] = m_sikous[m_winNums[1].second].GetValue(x, y);
                child_2[y][x] = m_sikous[m_winNums[0].second].GetValue(x, y);
            }
        }

        // 角の評価値は勝利数の多い方にする
        child_1[1][1] = m_sikous[m_winNums[0].second].GetValue(1, 1);
        child_2[1][1] = m_sikous[m_winNums[0].second].GetValue(1, 1);

        child_1[1][8] = m_sikous[m_winNums[0].second].GetValue(8, 1);
        child_2[1][8] = m_sikous[m_winNums[0].second].GetValue(8, 1);

        child_1[8][1] = m_sikous[m_winNums[0].second].GetValue(1, 8);
        child_2[8][1] = m_sikous[m_winNums[0].second].GetValue(1, 8);

        child_1[8][8] = m_sikous[m_winNums[0].second].GetValue(8, 8);
        child_2[8][8] = m_sikous[m_winNums[0].second].GetValue(8, 8);

        children.emplace_back(child_1);
        children.emplace_back(child_2);
    }

    // ランダムに選んだ二人を交配させる
    for (uint32_t i{}; i < (static_cast<uint32_t>(m_parentNum) - 4) / 2; ++i) {
        std::vector<Sikou> randomParents;
        uint32_t parentIndex_1{utl::RandomUint32(0, m_parentNum - 1)};
        uint32_t parentIndex_2{utl::RandomUint32(0, m_parentNum - 1)};
        randomParents.emplace_back(m_sikous[parentIndex_1]);
        randomParents.emplace_back(m_sikous[parentIndex_2]);

        std::array<std::array<int32_t, 10>, 10> child_3, child_4;

        uint32_t separateX{utl::RandomUint32(1, 8)};
        uint32_t separateY{utl::RandomUint32(1, 8)};

        for (uint32_t y{1}; y < separateY; ++y) {
            for (uint32_t x{1}; x <= 8; ++x) {
                child_3[y][x] = randomParents[0].GetValue(x, y);
                child_4[y][x] = randomParents[1].GetValue(x, y);
            }
        }

        for (uint32_t x{1}; x < separateX; ++x) {
            child_3[separateY][x] = randomParents[0].GetValue(x, separateY);
            child_4[separateY][x] = randomParents[1].GetValue(x, separateY);
        }
        for (uint32_t x{separateX}; x <= 8; ++x) {
            child_3[separateY][x] = randomParents[1].GetValue(x, separateY);
            child_4[separateY][x] = randomParents[0].GetValue(x, separateY);
        }

        for (uint32_t y{separateY + 1}; y <= 8; ++y) {
            for (uint32_t x{1}; x <= 8; ++x) {
                child_3[y][x] = randomParents[1].GetValue(x, y);
                child_4[y][x] = randomParents[0].GetValue(x, y);
            }
        }

        const auto& excellentParent = ((parentIndex_1 <= parentIndex_2) ? randomParents[0] : randomParents[1]);

        //// 角の評価値は勝利数の多い方にする
        //child_3[1][1] = excellentParent.GetValue(1, 1);
        //child_4[1][1] = excellentParent.GetValue(1, 1);

        //child_3[1][8] = excellentParent.GetValue(8, 1);
        //child_4[1][8] = excellentParent.GetValue(8, 1);

        //child_3[8][1] = excellentParent.GetValue(1, 8);
        //child_4[8][1] = excellentParent.GetValue(1, 8);

        //child_3[8][8] = excellentParent.GetValue(8, 8);
        //child_4[8][8] = excellentParent.GetValue(8, 8);

        // 角の評価値は最大値にする
        child_3[1][1] = 128;
        child_4[1][1] = 128;

        child_3[1][8] = 128;
        child_4[1][8] = 128;

        child_3[8][1] = 128;
        child_4[8][1] = 128;

        child_3[8][8] = 128;
        child_4[8][8] = 128;

        children.emplace_back(child_3);
        children.emplace_back(child_4);
    }

    m_sikous = children;
    for (auto& winNum : m_winNums) {
        winNum.first = 0;
    }
    Init(Turn::Player);
}

void bnsGame::Study::PlayerTurn() {
    m_aspect.Put(Self, m_sikous[m_playerIndex].Think(Self, m_aspect), m_sikous[m_playerIndex].GetValues());

    ChangeTurn();
}

void bnsGame::Study::EnemyTurn() {
    m_aspect.Put(Enemy, m_sikous[m_enemyIndex].Think(Enemy, m_aspect), m_sikous[m_enemyIndex].GetValues());

    ChangeTurn();
}

void bnsGame::Study::Result() {
    const auto winner = m_aspect.GetWinner(true);
    if (winner == Winner::Player) {
        ++m_winNums[m_playerIndex].first;
    }
    else if (winner == Winner::Enemy) {
        ++m_winNums[m_enemyIndex].first;
    }

    if (m_enemyIndex == (m_parentNum - 1)) {
        m_enemyIndex = 0;
        ++m_playerIndex;
        Init(Turn::Player);
        return;
    }

    ++m_enemyIndex;
    if (m_playerIndex == m_enemyIndex) {
        if (m_enemyIndex == (m_parentNum - 1)) {
            m_state = StudyState::Mating;
            m_enemyIndex = 0;
            m_playerIndex = 0;

            /*std::cout << "各インデックスの勝利数は、" << std::endl;
            for (const auto& winNum : m_winNums) {
                std::cout << winNum.first << " ";
            }
            std::cout << "です。" << std::endl;*/
            ++m_geneNum;

            if (m_geneNum <= 10) {
                Mating();
                return;
            }

            uint32_t exitCondition = static_cast<uint32_t>((static_cast<double>(m_parentNum) - 1) * 1.8 - 1);
            //std::cout << exitCondition << std::endl;
            for (const auto& winNum : m_winNums) {
                if (winNum.first < exitCondition) {
                    continue;
                }

                // 学習した評価値を保存
                getData().SetValues(m_sikous[winNum.second].GetValues());

                //m_sikous[winNum.second].Print();
                std::cout << "\n学習終了しました。\nエンターキーを押してください。" << std::endl;

                std::string s;
                s = _getch();
                std::getline(std::cin, s);
                
                ChangeScene(Scene::Title);
                return;
            }

            Mating();
            return;
        }
        ++m_enemyIndex;
        Init(Turn::Player);
        return;
    }

    Init(Turn::Player);
    return;
}

void bnsGame::Study::Update() {
    if (m_isChangeScene) {
        ChangeScene(Scene::Title);
        return;
    }
    switch (m_turn) {
    case bnsGame::Turn::Player: {
        std::thread th(&bnsGame::Study::PlayerTurn, this);
        Sleep(0);
        // EnemyTurn();
        th.join();
        break;
    }
    case bnsGame::Turn::Enemy: {
        std::thread th(&bnsGame::Study::EnemyTurn, this);
        Sleep(0);
        // EnemyTurn();
        th.join();
        break;
    }
    case bnsGame::Turn::Result:
        Result();
        break;
    default:
        break;
    }
}

void bnsGame::Study::UpdateChangeOut() {}

void bnsGame::Study::Draw() const {}
