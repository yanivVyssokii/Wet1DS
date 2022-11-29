#include "worldcup23a1.h"

world_cup_t::world_cup_t()
{
	// TODO: Your code goes here
}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}

StatusType world_cup_t::add_team(int teamId, int points)
{
    if (teamId<=0||points<0){
        return StatusType::INVALID_INPUT;
    }
    try {
        Team *newTeam = new Team(teamId, points);
        if (m_teams->find(*newTeam)){
            delete newTeam;
            return StatusType::FAILURE;
        }
        m_teams->insert(nullptr,new Node<Team>(teamId,newTeam, nullptr, nullptr, nullptr),newTeam);
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId<=0){
        return StatusType::INVALID_INPUT;
    }

    try {
        Team *newTeam = new Team(teamId, 0);
        Node<Team>* teamNode = m_teams->find(*newTeam);
        if (!teamNode){
            delete newTeam;
            return StatusType::FAILURE;
        }
        if (teamNode->data->getPlayerCount()>0){
            delete newTeam;
            return StatusType::FAILURE;
        }
        m_teams->deleteNode(teamNode,teamNode->data);
        delete newTeam;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
    if(playerId<=0||teamId<=0||gamesPlayed<0||goals<0||cards<0){
        return StatusType::FAILURE;
    }
    if (gamesPlayed==0&&(goals<0||cards<0)){
        return StatusType::FAILURE;
    }
    try {
        Player *newPlayer = new Player(playerId,teamId,gamesPlayed,goals,cards,goalKeeper);
        if (m_playersById->find(*newPlayer)){
            delete newPlayer;
            return StatusType::FAILURE;
        }

        Team *newTeam = new Team(teamId, 0);
        if (!m_teams->find(*newTeam)){
            delete newTeam;
            return StatusType::FAILURE;
        }
        Team* teamPtr=m_teams->find(*newTeam)->data;
        delete newTeam;

        m_playersById->insert(nullptr,
                              new Node<Player>(playerId,newPlayer, nullptr, nullptr, nullptr),newPlayer);
        m_playersByStats->insert(nullptr,
                              new Node<Player>(playerId,newPlayer, nullptr, nullptr, nullptr),newPlayer);
        teamPtr->addPlayer(newPlayer);

        newPlayer->setTeam(teamPtr);
        newPlayer->setTeamGamesBeforeJoin(teamPtr->getGamesPlayed());

        teamPtr->setPowerRank(teamPtr->getPowerRank()+goals-cards);
        teamPtr->increasePlayerCount(1);
        if (goalKeeper) {
            teamPtr->addGoalKeeper(1);
        }
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
    if (playerId<=0){
        return StatusType::INVALID_INPUT;
    }

    try {
        Player *newPlayer = new Player(playerId,0,0,0,0, false);
        Node<Player>* playerNode = m_playersById->find(*newPlayer);
        if (!playerNode){
            delete newPlayer;
            return StatusType::FAILURE;
        }
        m_playersById->deleteNode(playerNode,playerNode->data);
        m_playersByStats->deleteNode(playerNode,playerNode->data);
        playerNode->data->getTeam()->getPlayersByStats()->deleteNode(playerNode,playerNode->data);
        playerNode->data->getTeam()->getPlayersByStats()->deleteNode(playerNode,playerNode->data);

        playerNode->data->getTeam()->setPowerRank(
                playerNode->data->getTeam()->getPowerRank()-playerNode->data->getGoals()+
                playerNode->data->getPlayerCards());
        playerNode->data->getTeam()->increasePlayerCount(-1);
        if (playerNode->data->isGoalKeeper()) {
            playerNode->data->getTeam()->addGoalKeeper(-1);
        }
        delete newPlayer;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	if (playerId<=0||gamesPlayed<0||scoredGoals<0||cardsReceived<0){
        return StatusType::INVALID_INPUT;
    }
    try{
        Player *newPlayer = new Player(playerId,0,0,0,0, false);
        Node<Player>* playerNode = m_playersById->find(*newPlayer);
        if (!playerNode){
            delete newPlayer;
            return StatusType::FAILURE;
        }
        playerNode->data->addCards(cardsReceived);
        playerNode->data->addGamesPlayed(gamesPlayed);
        playerNode->data->addGoals(scoredGoals);
        playerNode->data->getTeam()->setPowerRank(
                playerNode->data->getTeam()->getPowerRank()+scoredGoals-cardsReceived);
        delete newPlayer;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	if (teamId1<=0||teamId2<=0||teamId1==teamId2){
        return StatusType::INVALID_INPUT;
    }

    try {
        Team *newTeam1 = new Team(teamId1, 0);
        Team *newTeam2 = new Team(teamId2, 0);

        Node<Team> *teamNode1 = m_teams->find(*newTeam1);
        Node<Team> *teamNode2 = m_teams->find(*newTeam2);


        if (!teamNode1||!teamNode2||teamNode1->data->getPlayerCount()<11||teamNode2->data->getPlayerCount()<11||
            teamNode1->data->getGoalKeepersCount()<1||teamNode1->data->getGoalKeepersCount()<1){
            delete newTeam1;
            delete newTeam2;
            return StatusType::FAILURE;
        }

        if (teamNode1->data->getPowerRank()+teamNode1->data->getPoints()>
                teamNode2->data->getPowerRank()+teamNode2->data->getPoints()){
            teamNode1->data->addPoints(3);
        }
        if (teamNode1->data->getPowerRank()+teamNode1->data->getPoints()<
            teamNode2->data->getPowerRank()+teamNode2->data->getPoints()){
            teamNode2->data->addPoints(3);
        }
        if (teamNode1->data->getPowerRank()+teamNode1->data->getPoints()==
            teamNode2->data->getPowerRank()+teamNode2->data->getPoints()){
            teamNode1->data->addPoints(1);
            teamNode1->data->addPoints(1);
        }
        teamNode1->data->addGamePlayed();
        teamNode2->data->addGamePlayed();
    } catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	if (playerId<=0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    try{
        Player *newPlayer = new Player(playerId,0,0,0,0, false);
        Node<Player>* playerNode = m_playersById->find(*newPlayer);
        if (!playerNode){
            delete newPlayer;
            return output_t<int>(StatusType::FAILURE);
        }
        return output_t<int>(playerNode->data->getTeam()->getGamesPlayed()-playerNode->data->getTeamGamesBeforeJoin()
                                    +playerNode->data->getGamesPlayed());
    }catch (...){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
	return 30003;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	// TODO: Your code goes here
	return 2008;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	// TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	// TODO: Your code goes here
    output[0] = 4001;
    output[1] = 4002;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
	return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}

