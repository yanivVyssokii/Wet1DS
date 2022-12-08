//
// Created by User on 11/15/2022.
//

#include "Team.h"

Team::Team(int teamId, int point): m_id(teamId), m_points(point),m_powerRank(0),m_topScorerId(NULLPLAYER),
                                    m_topScorerCards(0),
                                    m_topScorerGoals(-1), m_playerCount(0),m_goalKeepersCount(0),
                                    m_gamesCounter(0), m_playersById(new AVLTree<Player>(isBiggerIdPlayer)),
                                    m_playersByStats(new AVLTree<Player>(isBiggerStats)),m_nextInKosher(nullptr),
                                    m_prevInKosher(nullptr)
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
int Team::getTopScorerCards() const {
    return m_topScorerCards;
}
void Team::setTopScorerCards(int TopScorerCards) {
    m_topScorerCards=TopScorerCards;
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
    m_playersById->insert(nullptr,m_playersById->getRoot(),player);
    m_playersByStats->insert(nullptr,m_playersByStats->getRoot(),player);
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

void Team::setPlayersById(AVLTree<Player> *playersById) {
    m_playersById = playersById;
}

void Team::setPlayersByStats(AVLTree<Player> *playersByStats) {
    m_playersByStats = playersByStats;
}

void Team::setTopScorerGoals(int goals) {
    m_topScorerGoals=goals;
}

int Team::getTopScorerGoals() const {
    return m_topScorerGoals;
}

Team *Team::getNextKosher() const {
    return m_nextInKosher;
}

Team *Team::getPrevKosher() const {
    return m_prevInKosher;
}

void Team::setNextKosher(Team *newClosest) {
    m_nextInKosher=newClosest;
}

void Team::setPrevKosher(Team *newClosest) {
    m_prevInKosher=newClosest;
}

Team::~Team() {
    delete m_playersById;
    delete m_playersByStats;
}

bool isBiggerIdTeam(Team& p1, Team& p2){
    return p1.getId()>p2.getId();
}
