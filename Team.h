//
// Created by User on 11/15/2022.
//

#ifndef WET1DS_TEAM_H
#define WET1DS_TEAM_H

#define NULLPLAYER -999

#include "AVLTree.h"
#include "Player.h"

class Team {
private:
    int m_id;

    int m_points;

    int m_powerRank;

    int m_topScorerId;

    int m_playerCount;

    int m_goalKeepersCount;

    int m_gamesCounter;

    AVLTree<Player>* m_playersById;

    AVLTree<Player>* m_playersByStats;
public:
    Team(int teamId, int point);

    int getId() const;

    int getPoints() const;

    int getPowerRank() const;

    int getTopScorerId() const;

    int getPlayerCount() const;

    int getGoalKeepersCount() const;

    int getGamesPlayed() const;

    AVLTree<Player>* getPlayersById() const;

    AVLTree<Player>* getPlayersByStats() const;

    void addPoints(int points);

    void setPowerRank(int powerRank);

    void setTopScorerId(int topScorerId);

    void increasePlayerCount(int extra);

    void addPlayer(Player* player);

    void addGoalKeeper(int extra);

    void addGamePlayed();
};


#endif //WET1DS_TEAM_H
