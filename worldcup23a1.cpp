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
    Team *newTeam;
    try {
        newTeam = new Team(teamId, points);
        Node<Team>* teamNode = m_teams->find(*newTeam);
        if (m_teams->find(*newTeam)){
            delete newTeam;
            return StatusType::FAILURE;
        }
        m_teams->insert(nullptr,new Node<Team>(teamId,newTeam, nullptr, nullptr, nullptr),newTeam);
    } catch (...) {
        delete newTeam;
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
    Player *newPlayer;
    try {
        newPlayer = new Player(playerId,teamId,gamesPlayed,goals,cards,goalKeeper);

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
        if (goals>m_topScorerGoals){
            m_topScorerGoals=goals;
            m_topScorerId=playerId;
        }
        if (goals>teamPtr->getTopScorerGoals()){
            teamPtr->setTopScorerGoals(goals);
            teamPtr->setTopScorerId(playerId);
        }
    } catch (...) {
        delete newPlayer;
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
        if (playerNode->data->getGoals()>m_topScorerGoals){
            m_topScorerGoals=m_playersByStats->inpre(m_playersByStats->getRoot())->data->getGoals();
            m_topScorerId=m_playersByStats->inpre(m_playersByStats->getRoot())->data->getGoals();
        }
        if (playerNode->data->getGoals()>playerNode->data->getTeam()->getTopScorerGoals()){
            playerNode->data->getTeam()->setTopScorerGoals(playerNode->data->getTeam()->getPlayersByStats()->
                                                                inpre(m_playersByStats->getRoot())->data->getGoals());
            playerNode->data->getTeam()->setTopScorerId(playerNode->data->getTeam()->getPlayersByStats()->
                                                             inpre(m_playersByStats->getRoot())->data->getId());
        }
        m_playersById->deleteNode(playerNode,playerNode->data);
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
        m_playersByStats->deleteNode(playerNode,playerNode->data);
        playerNode->data->addCards(cardsReceived);
        playerNode->data->addGamesPlayed(gamesPlayed);
        playerNode->data->addGoals(scoredGoals);
        playerNode->data->getTeam()->setPowerRank(
                playerNode->data->getTeam()->getPowerRank()+scoredGoals-cardsReceived);
        m_playersByStats->insert(nullptr,playerNode,playerNode->data);
        if (playerNode->data->getGoals()>m_topScorerGoals){
            m_topScorerGoals=playerNode->data->getGoals();
            m_topScorerId=playerId;
        }
        if (playerNode->data->getGoals()>playerNode->data->getTeam()->getTopScorerGoals()){
            playerNode->data->getTeam()->setTopScorerGoals(playerNode->data->getGoals());
            playerNode->data->getTeam()->setTopScorerId(playerId);
        }
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
    Team *newTeam1;
    Team *newTeam2;
    try {
        newTeam1 = new Team(teamId1, 0);
        newTeam2 = new Team(teamId2, 0);

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
        delete newTeam1;
        delete newTeam2;
    } catch(...) {
        delete newTeam1;
        delete newTeam2;
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
	if (teamId<=0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    try{
        Team *newTeam = new Team(teamId,0);
        Node<Team>* teamNode = m_teams->find(*newTeam);
        if (!teamNode){
            delete newTeam;
            return output_t<int>(StatusType::FAILURE);
        }
        return output_t<int>(teamNode->data->getPowerRank()+teamNode->data->getPoints());
    }catch (...){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
    if(newTeamId<=0||teamId1<=0||teamId2<=0||teamId1==teamId2){
        return StatusType::INVALID_INPUT;
    }
    Team *oldTeam1;
    Team *oldTeam2;
    Team *newTeam;
    try{
        oldTeam1 = new Team(teamId1,0);
        Node<Team>* teamNode1 = m_teams->find(*oldTeam1);
        oldTeam2 = new Team(teamId2,0);
        Node<Team>* teamNode2 = m_teams->find(*oldTeam2);
        newTeam = new Team(newTeamId,0);
        Node<Team>* newTeamNode = m_teams->find(*newTeam);
        if ((newTeamNode&&newTeamId!=teamId1&&newTeamId!=teamId2)||!teamNode1||!teamNode2){
            delete oldTeam1;
            delete oldTeam2;
            delete newTeam;
            return StatusType::FAILURE;
        }
        AVLTree<Player>* mergedPlayersById = merge(teamNode1->data->getPlayersById(),teamNode2->data->getPlayersById());
        AVLTree<Player>* mergedPlayersByStats = merge(teamNode1->data->getPlayersByStats(),
                                                      teamNode2->data->getPlayersByStats());
        m_teams->deleteNode(teamNode1,teamNode1->data);
        m_teams->deleteNode(teamNode2,teamNode2->data);

        newTeam->addPoints(teamNode1->data->getPoints()+teamNode2->data->getPoints());
        newTeam->setPowerRank(teamNode1->data->getPowerRank()+teamNode2->data->getPowerRank());
        newTeam->increasePlayerCount(teamNode1->data->getPlayerCount()+teamNode2->data->getPlayerCount());
        newTeam->addGoalKeeper(teamNode1->data->getGoalKeepersCount()+teamNode2->data->getGoalKeepersCount());
        newTeam->setPlayersById(mergedPlayersById);
        newTeam->setPlayersByStats(mergedPlayersByStats);

        if (teamNode1->data->getTopScorerGoals()>teamNode2->data->getTopScorerGoals()){
            newTeam->setTopScorerGoals(teamNode1->data->getTopScorerGoals());
            newTeam->setTopScorerId(teamNode1->data->getTopScorerId());
        }
        else{
            newTeam->setTopScorerGoals(teamNode2->data->getTopScorerGoals());
            newTeam->setTopScorerId(teamNode2->data->getTopScorerId());
        }
        Node<Player>* arr = new Node<Player>[newTeam->getPlayerCount()];
        int i=0;
        newTeam->getPlayersById()->inOrder(newTeam->getPlayersById()->getRoot(),arr,&i);
        for (int j=0; j<newTeam->getPlayerCount();j++)
        delete oldTeam1;
        delete oldTeam2;
    }catch(...){
        delete oldTeam1;
        delete oldTeam2;
        delete newTeam;
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	if (teamId==0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    if (teamId<0&&m_playerCount<=0){
        return output_t<int>(StatusType::FAILURE);
    }
    try{
        if (teamId<0){
            return output_t<int>(m_topScorerId);
        }
        Team *newTeam = new Team(teamId,0);
        Node<Team>* teamNode = m_teams->find(*newTeam);
        if (!teamNode||teamNode->data->getPlayerCount()<=0){
            delete newTeam;
            return output_t<int>(StatusType::FAILURE);
        }
        delete newTeam;
        return output_t<int>(teamNode->data->getTopScorerId());
    }catch(...){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
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

