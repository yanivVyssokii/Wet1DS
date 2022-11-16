//
// Created by User on 11/15/2022.
//

#ifndef WET1DS_PLAYER_H
#define WET1DS_PLAYER_H


class Player {
private:
    int m_id;

    int m_teamId;

    int m_gamesPlayed;

    int m_goals;

    int m_cards;

    bool m_goalKeeper;

public:
    Player(int playerId, int teamId, int gamesPlayed, int goals, int cards, bool goalKeeper);

    int getId() const;

    int getTeamId() const;

    int HowMuchGamesPlayed() const;

    int getPlayerCards() const;

    bool isGoalKeeper() const;

    void addGamesPlayed(int games);

    void addGoals(int goals);

    void addCards(int cards);

    void changeGoalKeeper(bool isGoalKeeper);
};


#endif //WET1DS_PLAYER_H
