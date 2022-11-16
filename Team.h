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

    AVLTree<Player>* m_players;
public:
    Team(int teamId, int point);

    int getId() const;

    int getPoints() const;

    int getPowerRank() const;

    int getTopScorerId() const;

    int getPlayerCount() const;

    AVLTree<Player>* getPlayers() const;

    void addPoints(int points);

    void setPowerRank(int powerRank);

    void setTopScorerId(int topScorerId);

    void increasePlayerCount();

    void addPlayer(Player* player);
};


#endif //WET1DS_TEAM_H
