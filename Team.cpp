//
// Created by User on 11/15/2022.
//

#include "Team.h"

Team::Team(int teamId, int point): m_id(teamId), m_points(point), m_playerCount(0),
                                    m_powerRank(0), m_topScorerId(NULLPLAYER),m_goalKeepersCount(0),
                                    m_gamesCounter(0), m_playersById(new AVLTree<Player>(isBiggerId)),
                                    m_playersByStats(new AVLTree<Player>(isBiggerStats))
{}

int Team::getId() const {
    return m_id;
}

int Team::getPoints() const {
    return m_points;
}

int Team::getPowerRank() const {
    return m_powerRank;
}

int Team::getTopScorerId() const {
    return m_topScorerId;
}

int Team::getPlayerCount() const {
    return m_playerCount;
}

AVLTree<Player> *Team::getPlayersById() const {
    return m_playersById;
}

void Team::addPoints(int points) {
    m_points+=points;
}

void Team::setPowerRank(int powerRank) {
    m_powerRank=powerRank;
}

void Team::setTopScorerId(int topScorerId) {
    m_topScorerId=topScorerId;
}

void Team::increasePlayerCount(int extra) {
    m_playerCount+=extra;
}

void Team::addPlayer(Player* player) {
    m_playersById->insert(nullptr,
                          new Node<Player>(player->getId(),player, nullptr, nullptr, nullptr),player);
    m_playersByStats->insert(nullptr,
                          new Node<Player>(player->getId(),player, nullptr, nullptr, nullptr),player);
}

AVLTree<Player> *Team::getPlayersByStats() const {
    return m_playersByStats;
}

int Team::getGoalKeepersCount() const {
    return m_goalKeepersCount;
}

void Team::addGoalKeeper(int extra) {
    m_goalKeepersCount+=extra;
}

int Team::getGamesPlayed() const {
    return m_gamesCounter;
}

void Team::addGamePlayed() {
    m_gamesCounter++;
}
