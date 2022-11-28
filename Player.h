//
// Created by User on 11/15/2022.
//

#ifndef WET1DS_PLAYER_H
#define WET1DS_PLAYER_H

#include "Team.h"

class Player {
private:
    int m_id;

    int m_teamId;

    int m_gamesPlayed;

    int m_goals;

    int m_cards;

    int m_teamGamesBeforeJoin;

    bool m_goalKeeper;

    Team* m_team;

public:
    Player(int playerId, int teamId, int gamesPlayed, int goals, int cards, bool goalKeeper);

    int getId() const;

    int getTeamId() const;

    int HowMuchGamesPlayed() const;

    int getPlayerCards() const;

    int getGoals() const;

    int getTeamGamesBeforeJoin() const;

    Team* getTeam() const;

    bool isGoalKeeper() const;

    void addGamesPlayed(int games);

    void addGoals(int goals);

    void addCards(int cards);

    void changeGoalKeeper(bool isGoalKeeper);

    void setTeam(Team* newTeam);

    void setTeamGamesBeforeJoin(int teamGamesBeforeJoin) ;
};

bool isBiggerId(Player& p1, Player& p2){
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
#endif //WET1DS_PLAYER_H
