#include "worldcup23a1.h"
#include "iostream"
world_cup_t::world_cup_t():m_teams(new AVLTree<Team>(isBiggerIdTeam)),
                            m_kosherTeams(new AVLTree<Team>(isBiggerIdTeam)),
                            m_playersById(new AVLTree<Player>(isBiggerIdPlayer)),
                            m_playersByStats(new AVLTree<Player>(isBiggerStats)),
                            m_topScorerId(0),
                            m_topScorerGoals(0),
                            m_topScorerCards(0),
                            m_playerCount(0)

{}

world_cup_t::~world_cup_t()
{
    m_playersById->deleteData(m_playersById->getRoot());
	delete m_playersByStats;
    delete m_playersById;
    m_teams->deleteData(m_teams->getRoot());
    delete m_kosherTeams;
    delete m_teams;

}

StatusType world_cup_t::add_team(int teamId, int points)
{
    if (teamId<=0||points<0){
        return StatusType::INVALID_INPUT;
    }
    Team *newTeam;
    try {
        newTeam = new Team(teamId, points);
        if (m_teams->find(*newTeam)){
            delete newTeam;
            return StatusType::FAILURE;
        }
        m_teams->insert(nullptr,m_teams->getRoot(),newTeam);
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
        Team *removedTeam=teamNode->data;
        m_teams->deleteNode(m_teams->getRoot(),teamNode->data);
        delete newTeam;
        delete removedTeam;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}
StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
    if(playerId<=0||teamId<=0||gamesPlayed<0||goals<0||cards<0){
        return StatusType::INVALID_INPUT;
    }
    if (gamesPlayed==0&&(goals>0||cards>0)){
        return StatusType::INVALID_INPUT;
    }
    Player *newPlayer= nullptr;
    Team *newTeam = nullptr;
    try {
        newPlayer = new Player(playerId,teamId,gamesPlayed,goals,cards,goalKeeper);

        if (m_playersById->find(*newPlayer)){
            delete newPlayer;
            return StatusType::FAILURE;
        }

        newTeam = new Team(teamId, 0);
        if (!m_teams->find(*newTeam)){
            delete newPlayer;
            delete newTeam;
            return StatusType::FAILURE;
        }
        Team* teamPtr=(m_teams->find(*newTeam))->data;
        delete newTeam;
        m_playersById->insert(nullptr,m_playersById->getRoot(),newPlayer);
        m_playersByStats->insert(nullptr,m_playersByStats->getRoot(),newPlayer);
        teamPtr->addPlayer(newPlayer);

        newPlayer->setTeam(teamPtr);
        newPlayer->setTeamGamesBeforeJoin(teamPtr->getGamesPlayed());

        teamPtr->setPowerRank(teamPtr->getPowerRank()+goals-cards);
        teamPtr->increasePlayerCount(1);
        if (goalKeeper) {
            teamPtr->addGoalKeeper(1);
        }
        if (goals>m_topScorerGoals||(goals==m_topScorerGoals&&cards<m_topScorerCards)
            ||(goals==m_topScorerGoals&&cards==m_topScorerCards
               &&playerId>m_topScorerId)){
            m_topScorerGoals=goals;
            m_topScorerId=playerId;
            m_topScorerCards=cards;
        }

        if (goals>teamPtr->getTopScorerGoals()||(goals==teamPtr->getTopScorerGoals()&&cards<teamPtr->getTopScorerCards())
                                                ||(goals==teamPtr->getTopScorerGoals()&&cards==teamPtr->getTopScorerCards()
                                                &&playerId>teamPtr->getTopScorerId())){
            teamPtr->setTopScorerGoals(goals);
            teamPtr->setTopScorerId(playerId);
            teamPtr->setTopScorerCards(cards);
        }

        m_playerCount++;

        if(m_playersByStats->findClosestSmaller(m_playersByStats->find(*newPlayer))){
            newPlayer->setClosestPlayerLeft(m_playersByStats->findClosestSmaller(m_playersByStats->find(*newPlayer))->data);
        }
        if(m_playersByStats->findClosestBigger(m_playersByStats->find(*newPlayer))) {
            newPlayer->setClosestPlayerRight(m_playersByStats->findClosestBigger(m_playersByStats->find(*newPlayer))->data);
        }
        if (newPlayer->getClosestPlayerLeft()) {
            newPlayer->getClosestPlayerLeft()->setClosestPlayerRight(newPlayer);
        }
        if (newPlayer->getClosestPlayerRight()) {
            newPlayer->getClosestPlayerRight()->setClosestPlayerLeft(newPlayer);
        }
        if (teamPtr->getPlayerCount()==11&&teamPtr->getGoalKeepersCount()>=1){
            m_kosherTeams->insert(nullptr,m_kosherTeams->getRoot(),teamPtr);
            if (m_kosherTeams->findClosestBigger(m_kosherTeams->find(*teamPtr))) {
                teamPtr->setNextKosher(m_kosherTeams->findClosestBigger(m_kosherTeams->find(*teamPtr))->data);
            }
            if (m_kosherTeams->findClosestSmaller(m_kosherTeams->find(*teamPtr))) {
                teamPtr->setPrevKosher(m_kosherTeams->findClosestSmaller(m_kosherTeams->find(*teamPtr))->data);
            }
            if (teamPtr->getNextKosher()) {
                teamPtr->getNextKosher()->setPrevKosher(teamPtr);
            }
            if (teamPtr->getPrevKosher()){
                teamPtr->getPrevKosher()->setNextKosher(teamPtr);
            }
        }
    } catch (...) {
        delete newPlayer;
        delete newTeam;
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
        Player *removedPlayer=playerNode->data;
        m_playersByStats->deleteNode(m_playersByStats->getRoot(),removedPlayer);
        removedPlayer->getTeam()->getPlayersByStats()->deleteNode(  removedPlayer->getTeam()
                                                ->getPlayersByStats()->getRoot(),removedPlayer);
        removedPlayer->getTeam()->getPlayersById()->deleteNode(removedPlayer->getTeam()
                                                ->getPlayersById()->getRoot(),removedPlayer);

        removedPlayer->getTeam()->setPowerRank(
                removedPlayer->getTeam()->getPowerRank()-removedPlayer->getGoals()+
                        removedPlayer->getPlayerCards());
        removedPlayer->getTeam()->increasePlayerCount(-1);
        m_playerCount--;

        if (removedPlayer->isGoalKeeper()) {
            removedPlayer->getTeam()->addGoalKeeper(-1);
        }
        if (removedPlayer->getTeam()->getPlayerCount()==0){
            removedPlayer->getTeam()->setTopScorerGoals(-1);
            removedPlayer->getTeam()->setTopScorerId(0);
            removedPlayer->getTeam()->setTopScorerCards(0);
        }
        if (m_playerCount==0) {
            m_topScorerGoals=-1;
            m_topScorerId=0;
            m_topScorerCards=0;
        }
        if (m_playerCount!=0&&removedPlayer->getId()==m_topScorerId){

            m_topScorerGoals=m_playersByStats->inpre(m_playersByStats->getRoot())->data->getGoals();
            m_topScorerId=m_playersByStats->inpre(m_playersByStats->getRoot())->data->getId();
            m_topScorerCards=m_playersByStats->inpre(m_playersByStats->getRoot())->data->getPlayerCards();
        }
        if (removedPlayer->getTeam()->getPlayerCount()!=0&&
                removedPlayer->getId()==removedPlayer->getTeam()->getTopScorerId()){

            removedPlayer->getTeam()->setTopScorerGoals(removedPlayer->getTeam()->getPlayersByStats()->
                                                                inpre(m_playersByStats->getRoot())->data->getGoals());
            removedPlayer->getTeam()->setTopScorerId(removedPlayer->getTeam()->getPlayersByStats()->
                                                             inpre(m_playersByStats->getRoot())->data->getId());
            removedPlayer->getTeam()->setTopScorerCards(removedPlayer->getTeam()->getPlayersByStats()->
                                        inpre(m_playersByStats->getRoot())->data->getPlayerCards());
        }
        if(removedPlayer->getClosestPlayerLeft()) {
            removedPlayer->getClosestPlayerLeft()->setClosestPlayerRight(removedPlayer->getClosestPlayerRight());
        }
        if (removedPlayer->getClosestPlayerRight()) {
            removedPlayer->getClosestPlayerRight()->setClosestPlayerLeft(removedPlayer->getClosestPlayerLeft());
        }
        if ( removedPlayer->getTeam()->getPlayerCount()==10|| removedPlayer->getTeam()->getGoalKeepersCount()==0){
            m_kosherTeams->deleteNode(m_kosherTeams->getRoot(), removedPlayer->getTeam());
        }
        m_playersById->deleteNode(m_playersById->getRoot(),removedPlayer);
        delete removedPlayer;
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
        Player *realPlayer = new Player(playerId,playerNode->data->getTeamId(),playerNode->data->getGamesPlayed(),
                                        playerNode->data->getGoals(),playerNode->data->getPlayerCards(),
                                        playerNode->data->isGoalKeeper());
        realPlayer->setTeam(playerNode->data->getTeam());
        realPlayer->setTeamGamesBeforeJoin(playerNode->data->getTeamGamesBeforeJoin());

        realPlayer->addCards(cardsReceived);
        realPlayer->addGamesPlayed(gamesPlayed);
        realPlayer->addGoals(scoredGoals);
        realPlayer->getTeam()->setPowerRank(
                realPlayer->getTeam()->getPowerRank()+scoredGoals-cardsReceived);

        Player *removedPlayer=playerNode->data;
        m_playersByStats->deleteNode(m_playersByStats->getRoot(),removedPlayer);
        removedPlayer->getTeam()->getPlayersByStats()->deleteNode(removedPlayer->getTeam()->
                                            getPlayersByStats()->getRoot(),removedPlayer);
        removedPlayer->getTeam()->getPlayersById()->deleteNode(removedPlayer->getTeam()->
                getPlayersById()->getRoot(),removedPlayer);
        Player* deletePlayer = removedPlayer;

        if(removedPlayer->getClosestPlayerLeft()) {
            removedPlayer->getClosestPlayerLeft()->setClosestPlayerRight(removedPlayer->getClosestPlayerRight());
        }
        if (removedPlayer->getClosestPlayerRight()) {
            removedPlayer->getClosestPlayerRight()->setClosestPlayerLeft(removedPlayer->getClosestPlayerLeft());
        }

        m_playersById->deleteNode(m_playersById->getRoot(),removedPlayer);
        delete deletePlayer;



        m_playersByStats->insert(nullptr,m_playersByStats->getRoot(),realPlayer);
        m_playersById->insert(nullptr,m_playersById->getRoot(),realPlayer);
        realPlayer->getTeam()->getPlayersById()->insert(nullptr,realPlayer->getTeam()->
                getPlayersById()->getRoot(),realPlayer);
        realPlayer->getTeam()->getPlayersByStats()->insert(nullptr,realPlayer->getTeam()->
                getPlayersByStats()->getRoot(),realPlayer);


         if(m_playersByStats->findClosestSmaller(m_playersByStats->find(*realPlayer))){
            realPlayer->setClosestPlayerLeft(m_playersByStats->findClosestSmaller(m_playersByStats->find(*realPlayer))->data);
        }
        if(m_playersByStats->findClosestBigger(m_playersByStats->find(*realPlayer))) {
            realPlayer->setClosestPlayerRight(m_playersByStats->findClosestBigger(m_playersByStats->find(*realPlayer))->data);
        }
        if (realPlayer->getClosestPlayerLeft()) {
            realPlayer->getClosestPlayerLeft()->setClosestPlayerRight(realPlayer);
        }
        if (realPlayer->getClosestPlayerRight()) {
            realPlayer->getClosestPlayerRight()->setClosestPlayerLeft(realPlayer);
        }
        if (realPlayer->getGoals()>m_topScorerGoals||(realPlayer->getGoals()==m_topScorerGoals&&realPlayer->getPlayerCards()<m_topScorerCards)
            ||(realPlayer->getGoals()==m_topScorerGoals&&realPlayer->getPlayerCards()==m_topScorerCards
               &&realPlayer->getId()>m_topScorerId)){
            m_topScorerId=realPlayer->getId();
            m_topScorerGoals=realPlayer->getGoals();
            m_topScorerCards=realPlayer->getPlayerCards();
        }

        if (realPlayer->getGoals()>realPlayer->getTeam()->getTopScorerGoals()||(realPlayer->getGoals()==realPlayer->getTeam()->getTopScorerGoals()&&
                        realPlayer->getPlayerCards()<realPlayer->getTeam()->getTopScorerCards())
                        ||(realPlayer->getGoals()==realPlayer->getTeam()->getTopScorerGoals()&&realPlayer->getPlayerCards()==realPlayer->getTeam()->getTopScorerCards()
                        &&realPlayer->getId()>realPlayer->getTeam()->getTopScorerId())){
            realPlayer->getTeam()->setTopScorerGoals(realPlayer->getGoals());
            realPlayer->getTeam()->setTopScorerId(realPlayer->getId());
            realPlayer->getTeam()->setTopScorerCards(realPlayer->getPlayerCards());
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
            teamNode1->data->getGoalKeepersCount()<1||teamNode2->data->getGoalKeepersCount()<1){
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
        delete newPlayer;
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
    Team *newTeam= nullptr;
    try{
       newTeam = new Team(teamId,0);
        Node<Team>* teamNode = m_teams->find(*newTeam);
        if (!teamNode){
            delete newTeam;
            return output_t<int>(StatusType::FAILURE);
        }
        delete newTeam;
        return output_t<int>(teamNode->data->getPoints());
    }catch (...){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
    if(newTeamId<=0||teamId1<=0||teamId2<=0||teamId1==teamId2){
        return StatusType::INVALID_INPUT;
    }
    Team *oldTeam1=nullptr;
    Team *oldTeam2=nullptr;
    Team *newTeam=nullptr;
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
        mergedPlayersById->updateTeam(mergedPlayersById->getRoot(),newTeam,newTeamId);
        AVLTree<Player>* mergedPlayersByStats = merge(teamNode1->data->getPlayersByStats(),
                                                      teamNode2->data->getPlayersByStats());

        newTeam->addPoints(teamNode1->data->getPoints()+teamNode2->data->getPoints());
        newTeam->setPowerRank(teamNode1->data->getPowerRank()+teamNode2->data->getPowerRank());
        newTeam->increasePlayerCount(teamNode1->data->getPlayerCount()+teamNode2->data->getPlayerCount());
        newTeam->addGoalKeeper(teamNode1->data->getGoalKeepersCount()+teamNode2->data->getGoalKeepersCount());
        delete newTeam->getPlayersByStats();
        delete newTeam->getPlayersById();
        newTeam->setPlayersById(nullptr);
        newTeam->setPlayersByStats(nullptr);
        newTeam->setPlayersById(mergedPlayersById);
        newTeam->setPlayersByStats(mergedPlayersByStats);

        if ((teamNode1->data->getTopScorerGoals()>teamNode2->data->getTopScorerGoals())||
                        (teamNode1->data->getTopScorerGoals()==teamNode2->data->getTopScorerGoals()&&
                        teamNode1->data->getTopScorerCards()<teamNode2->data->getTopScorerCards())||
                        (teamNode1->data->getTopScorerGoals()==teamNode2->data->getTopScorerGoals()&&
                         teamNode1->data->getTopScorerCards()==teamNode2->data->getTopScorerCards()&&
                         teamNode1->data->getTopScorerId()>teamNode2->data->getTopScorerId())){
            newTeam->setTopScorerGoals(teamNode1->data->getTopScorerGoals());
            newTeam->setTopScorerId(teamNode1->data->getTopScorerId());
            newTeam->setTopScorerCards(teamNode1->data->getTopScorerCards());
        }
        else{
            newTeam->setTopScorerGoals(teamNode2->data->getTopScorerGoals());
            newTeam->setTopScorerId(teamNode2->data->getTopScorerId());
            newTeam->setTopScorerCards(teamNode2->data->getTopScorerCards());
        }
        Team* removedTeam1=teamNode1->data;
        Team* removedTeam2=teamNode2->data;
        m_teams->deleteNode(m_teams->getRoot(),removedTeam1);
        m_teams->deleteNode(m_teams->getRoot(),removedTeam2);
        m_kosherTeams->deleteNode(m_kosherTeams->getRoot(), oldTeam1);
        m_kosherTeams->deleteNode(m_kosherTeams->getRoot(), oldTeam2);

        m_teams->insert(nullptr,m_teams->getRoot(),newTeam);
        if ( newTeam->getPlayerCount()>=11&& newTeam->getGoalKeepersCount()>=1){
            m_kosherTeams->insert(nullptr,m_kosherTeams->getRoot(),newTeam);
            if (m_kosherTeams->findClosestBigger(m_kosherTeams->find(*newTeam))) {
                newTeam->setNextKosher(m_kosherTeams->findClosestBigger(m_kosherTeams->find(*newTeam))->data);
            }
            if (m_kosherTeams->findClosestSmaller(m_kosherTeams->find(*newTeam))) {
                newTeam->setPrevKosher(m_kosherTeams->findClosestSmaller(m_kosherTeams->find(*newTeam))->data);
            }

        }
        delete removedTeam1;
        delete removedTeam2;
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
	if (teamId==0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    if (teamId<0){
        return m_playerCount;
    }
    Team *newTeam= nullptr;
    try{
        newTeam = new Team(teamId,0);
        Node<Team>* teamNode = m_teams->find(*newTeam);
        if (!teamNode){
            delete newTeam;
            return output_t<int>(StatusType::FAILURE);
        }
        delete newTeam;
        return output_t<int>(teamNode->data->getPlayerCount());
    }catch(...){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	if (teamId==0||!output){
        return StatusType::INVALID_INPUT;
    }
    if (teamId<0&&m_playerCount<=0){
        return StatusType::FAILURE;
    }
    Node<Player>** arr;
    Team *newTeam;
    try {
        if (teamId < 0) {
            newTeam= nullptr;
            arr=new Node<Player>*[m_playerCount];
            int index=0;
            m_playersByStats->inOrder(m_playersByStats->getRoot(),arr,&index);
            for (int i=0; i<m_playerCount; i++){
                output[i]=arr[i]->data->getId();
            }
            delete[] arr;
            return StatusType::SUCCESS;
        }
        newTeam = new Team(teamId,0);
        Node<Team>* teamNode = m_teams->find(*newTeam);
        if (!teamNode){
            delete newTeam;
            return StatusType::FAILURE;
        }
        if (teamNode->data->getPlayerCount()<=0){
            delete newTeam;
            return StatusType::FAILURE;
        }
        arr=new Node<Player>*[teamNode->data->getPlayerCount()];
        int index=0;
        teamNode->data->getPlayersByStats()->inOrder(teamNode->data->getPlayersByStats()->getRoot(),arr,&index);
        for (int i=0; i<teamNode->data->getPlayerCount(); i++){
            output[i]=arr[i]->data->getId();
        }
        newTeam->getPlayersByStats()->setRoot(nullptr);
        newTeam->getPlayersById()->setRoot(nullptr);
        delete newTeam;
        delete[] arr;
        return StatusType::SUCCESS;
    }catch(...){
        delete newTeam;
        return StatusType::ALLOCATION_ERROR;
    }
}

int abs(int a, int b){
    if (b-a>0){
        return b-a;
    }
    return a-b;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
    if (playerId<=0||teamId<=0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    Team *newTeam;
    try{
        newTeam= nullptr;
        newTeam = new Team(teamId,0);
        Node<Team>* teamNode = m_teams->find(*newTeam);
        if(!teamNode){
            delete newTeam;
            return output_t<int>(StatusType::FAILURE);
        }
        Player *newPlayer = new Player(playerId,0,0,0,0, false);
        Node<Player>* playerNode = teamNode->data->getPlayersById()->find(*newPlayer);
        if (!playerNode||m_playerCount==1){
            delete newTeam;
            delete newPlayer;
            return output_t<int>(StatusType::FAILURE);
        }
        Player* bigger = playerNode->data->getClosestPlayerRight();
        Player* smaller = playerNode->data->getClosestPlayerLeft();
        delete newTeam;
        delete newPlayer;
        if (!smaller)
            return output_t<int>(bigger->getId());
        if (!bigger)
            return output_t<int>(smaller->getId());
        if (abs(smaller->getGoals(),playerNode->data->getGoals())>abs(bigger->getGoals(),
                                                                            playerNode->data->getGoals())){
            return output_t<int>(bigger->getId());
        }
        if (abs(smaller->getGoals(),playerNode->data->getGoals())<abs(bigger->getGoals(),
                                                                            playerNode->data->getGoals())){
            return output_t<int>(smaller->getId());
        }
        if (abs(smaller->getPlayerCards(),playerNode->data->getPlayerCards())>abs(bigger->getPlayerCards(),
                                                                            playerNode->data->getPlayerCards())){
            return output_t<int>(bigger->getId());
        }
        if (abs(smaller->getPlayerCards(),playerNode->data->getPlayerCards())<abs(bigger->getPlayerCards(),
                                                                            playerNode->data->getPlayerCards())){
            return output_t<int>(smaller->getId());
        }
        if (abs(smaller->getId(),playerNode->data->getId())>abs(bigger->getId(),playerNode->data->getId())){
            return output_t<int>(bigger->getId());
        }
        if (abs(smaller->getId(),playerNode->data->getId())<abs(bigger->getId(),playerNode->data->getId())){
            return output_t<int>(smaller->getId());
        }
        return (bigger->getId()>smaller->getId()? output_t<int>(bigger->getId())
                :output_t<int>(smaller->getId()));
    } catch(...){
        delete newTeam;
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }

}



output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
    if (maxTeamId<minTeamId||maxTeamId<0||minTeamId<0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    int* arrId= nullptr;
    int* arrPoints= nullptr;
    int size=1;
    if (m_kosherTeams->getSize()==0||m_kosherTeams->inpre(m_kosherTeams->getRoot())->data->getId()<minTeamId||
            m_kosherTeams->insuc(m_kosherTeams->getRoot())->data->getId()>maxTeamId) {
        return output_t<int>(StatusType::FAILURE);
    }
    try{
        if (!findRangeTeam(minTeamId,maxTeamId,&size,arrId,arrPoints)){
            return output_t<int>(StatusType::FAILURE);
        }
        int index=0;
        int diff=1;
        while (arrId[0]!=arrId[size-1]){
            for (index=0;index<size;index+=diff){
                if (index+diff>=size){
                    continue;
                }
                if (arrPoints[index]<=arrPoints[index+diff]){
                    arrId[index]=arrId[index+diff];
                }
                else {
                    arrId[index+diff]=arrId[index];
                }
                arrPoints[index]+=arrPoints[index+diff];
            }
            diff*=2;
        }

        int winningId=arrId[0];
        delete []arrId;
        delete []arrPoints;
        return output_t<int>(winningId);
    } catch(...){
        delete arrId;
        delete arrPoints;
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }

}



bool world_cup_t::findRangeTeam(int min, int max, int* size,int*& ids, int*& points) {
    Node<Team>* closestToMin;
    Node<Team>* closestToMax;
    Node<Team>* current=m_kosherTeams->getRoot();
    while(true){
        if (current->data->getId()==min){
            closestToMin=current;
            break;
        }
        if (current->data->getId()>min){
            if (current->left==nullptr){
                closestToMin=current;
                break;
            }
            current=current->left;
        }
        else{
            if (current->right==nullptr){
                closestToMin=current;
                break;
            }
            current=current->right;
        }
    }
    if (current->data->getId()<min){
        closestToMin=m_kosherTeams->findClosestBigger(closestToMin);
        while (true){
            if (current==nullptr){
                return false;
            }
            if (current->father->left==current){
                closestToMin=current->father;
                break;
            }
            current=current->father;
        }
    }
    current=m_kosherTeams->getRoot();
    while(true){
        if (current->data->getId()==max){
            closestToMax=current;
            break;
        }
        if (current->data->getId()>max){
            if (current->left==nullptr){
                closestToMax=current;
                break;
            }
            current=current->left;
        }
        else{
            if (current->right==nullptr){
                closestToMax=current;
                break;
            }
            current=current->right;
        }
    }
    if (current->data->getId()>max){

        while (true){
            if (!current){
                return false;
            }
            if (current->father->right==current){
                closestToMax=current->father;
                break;
            }
            current=current->father;
        }
    }

    //we have min max node
    Team* currentTeam=closestToMin->data;
    *size=1;
    while(true){
        if(currentTeam== nullptr){
            return false;
        }
        if (currentTeam->getId()==closestToMax->data->getId()){
            break;
        }
        currentTeam=currentTeam->getNextKosher();
        (*size)++;
    }
    ids = new int[*size];
    points = new int[*size];
    currentTeam=closestToMin->data;
    int index=0;
    while(true){
        ids[index]=currentTeam->getId();
        points[index]=currentTeam->getPowerRank()+currentTeam->getPoints();
        index++;
        if (currentTeam->getId()==closestToMax->data->getId()){
            break;
        }
        currentTeam=currentTeam->getNextKosher();

    }
    return true;
}

