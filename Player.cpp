//
// Created by User on 11/15/2022.
//

#include "Player.h"

int Player::getId() const {
    return m_id;
}

int Player::getTeamId() const {
    return m_teamId;
}


int Player::getPlayerCards() const {
    return m_cards;
}

bool Player::isGoalKeeper() const {
    return m_goalKeeper;
}

void Player::addGamesPlayed(int games) {
    m_gamesPlayed+=games;
}

void Player::addGoals(int goals) {
    m_goals+=goals;
}

void Player::addCards(int cards) {
    m_cards+=cards;
}


Player::Player(int playerId, int teamId, int gamesPlayed, int goals, int cards, bool goalKeeper):
                m_id(playerId), m_teamId(teamId), m_gamesPlayed(gamesPlayed), m_goals(goals), m_cards(cards),
                m_teamGamesBeforeJoin(0), m_goalKeeper(goalKeeper),m_closestRight(nullptr),m_closestLeft(nullptr),
                m_team(nullptr)
{}

int Player::getGoals() const {
    return m_goals;
}

void Player::setTeam(Team *newTeam) {
    m_team=newTeam;
}

Team *Player::getTeam() const {
    return m_team;
}

void Player::setTeamGamesBeforeJoin(int teamGamesBeforeJoin) {
    m_teamGamesBeforeJoin=teamGamesBeforeJoin;
}

int Player::getTeamGamesBeforeJoin() const {
    return m_teamGamesBeforeJoin;
}

int Player::getGamesPlayed() const {
    return m_gamesPlayed;
}

Player *Player::getClosestPlayerRight() const {
    return m_closestRight;
}

void Player::setClosestPlayerRight(Player *newClosest) {
    m_closestRight=newClosest;
}

Player *Player::getClosestPlayerLeft() const {
    return m_closestLeft;
}

void Player::setClosestPlayerLeft(Player *newClosest) {
    m_closestLeft=newClosest;
}

void Player::setTeamId(int id) {
    m_teamId=id;

}


bool isBiggerIdPlayer(Player& p1, Player& p2){
    return p1.getId()>p2.getId();
}

bool isBiggerStats(Player& p1, Player& p2){
    if (p1.getGoals()>p2.getGoals()){
        return true;
    }
    if (p2.getGoals()>p1.getGoals()){
        return false;
    }
    if (p2.getPlayerCards()>p1.getPlayerCards()){
        return true;
    }
    if (p1.getPlayerCards()>p2.getPlayerCards()){
        return false;
    }
    return p1.getId()>p2.getId();
}

