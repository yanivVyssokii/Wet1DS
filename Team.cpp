//
// Created by User on 11/15/2022.
//

#include "Team.h"

Team::Team(int teamId, int point): m_teamId(teamId), m_points(point), m_playerCount(0),
                                    m_powerRank(0), m_topScorerId(NULLPLAYER), m_players(new AVLTree<Player>())
{}

int Team::getTeamId() const {
    return m_teamId;
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

AVLTree<Player> *Team::getPlayers() const {
    return m_players;
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

void Team::increasePlayerCount() {
    m_playerCount++;
}

void Team::addPlayer(const Player &player) {
    //m_players->add(new Node<Player>(player.m_teamId,)) TODO finish adding
}
