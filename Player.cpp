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

int Player::HowMuchGamesPlayed() const {
    return m_gamesPlayed;
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

void Player::changeGoalKeeper(bool isGoalKeeper) {
    m_goalKeeper=isGoalKeeper;
}

Player::Player(int playerId, int teamId, int gamesPlayed, int goals, int cards, bool goalKeeper):
                m_id(playerId), m_teamId(teamId), m_gamesPlayed(gamesPlayed), m_goals(goals), m_cards(cards),
                m_goalKeeper(goalKeeper)
{}
