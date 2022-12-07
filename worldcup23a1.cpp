#include "worldcup23a1.h"

world_cup_t::world_cup_t():m_playersByStats(new AVLTree<Player>(isBiggerStats)),m_playersById(new AVLTree<Player>(isBiggerIdPlayer)),
                            m_kosherTeams(new AVLTree<Team>(isBiggerIdTeam)),m_teams(new AVLTree<Team>(isBiggerIdTeam)),m_playerCount(0),
                            m_topScorerGoals(0),m_topScorerId(0)
{}

world_cup_t::~world_cup_t()
{
	delete m_playersByStats;
    m_playersById->deleteData(m_playersById->getRoot());
    delete m_playersById;
    delete m_kosherTeams;
    m_teams->deleteData(m_teams->getRoot());
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
        Node<Team>* teamNode = m_teams->find(*newTeam);
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
        if (goals>m_topScorerGoals){
            m_topScorerGoals=goals;
            m_topScorerId=playerId;
        }
        if (goals>teamPtr->getTopScorerGoals()){
            teamPtr->setTopScorerGoals(goals);
            teamPtr->setTopScorerId(playerId);
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
        m_playersByStats->deleteNode(m_playersByStats->getRoot(),playerNode->data);
        playerNode->data->getTeam()->getPlayersByStats()->deleteNode(  playerNode->data->getTeam()
                                                ->getPlayersByStats()->getRoot(),playerNode->data);
        playerNode->data->getTeam()->getPlayersById()->deleteNode(playerNode->data->getTeam()
                                                ->getPlayersById()->getRoot(),playerNode->data);

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
        if(playerNode->data->getClosestPlayerLeft()) {
            playerNode->data->getClosestPlayerLeft()->setClosestPlayerRight(playerNode->data->getClosestPlayerRight());
        }
        if (playerNode->data->getClosestPlayerRight()) {
            playerNode->data->getClosestPlayerRight()->setClosestPlayerLeft(playerNode->data->getClosestPlayerLeft());
        }
        m_playersById->deleteNode(m_playersById->getRoot(),playerNode->data);
        m_playerCount--;
        if ( playerNode->data->getTeam()->getPlayerCount()==10|| playerNode->data->getTeam()->getGoalKeepersCount()==0){
            m_kosherTeams->deleteNode(m_kosherTeams->getRoot(), playerNode->data->getTeam());
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
        Player *realPlayer = new Player(playerId,playerNode->data->getTeamId(),playerNode->data->getGamesPlayed(),
                                        playerNode->data->getGoals(),playerNode->data->getPlayerCards(),
                                        playerNode->data->isGoalKeeper());
        realPlayer->setTeam(playerNode->data->getTeam());
        realPlayer->setTeamGamesBeforeJoin(playerNode->data->getTeamGamesBeforeJoin());
        m_playersByStats->deleteNode(playerNode,playerNode->data);
        realPlayer->addCards(cardsReceived);
        realPlayer->addGamesPlayed(gamesPlayed);
        realPlayer->addGoals(scoredGoals);
        realPlayer->getTeam()->setPowerRank(
                realPlayer->getTeam()->getPowerRank()+scoredGoals-cardsReceived);
        m_playersByStats->insert(nullptr,m_playersByStats->getRoot(),realPlayer);
        if (realPlayer->getGoals()>m_topScorerGoals){
            m_topScorerGoals=realPlayer->getGoals();
            m_topScorerId=playerId;
        }
        if (realPlayer->getGoals()>realPlayer->getTeam()->getTopScorerGoals()){
            realPlayer->getTeam()->setTopScorerGoals(realPlayer->getGoals());
            realPlayer->getTeam()->setTopScorerId(playerId);
        }
        delete newPlayer;
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
        m_teams->deleteNode(m_teams->getRoot(),teamNode1->data);
        m_teams->deleteNode(m_teams->getRoot(),teamNode2->data);
        m_kosherTeams->deleteNode(m_kosherTeams->getRoot(), oldTeam1);
        m_kosherTeams->deleteNode(m_kosherTeams->getRoot(), oldTeam2);

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
        m_teams->insert(nullptr,m_teams->getRoot(),newTeam);
        if ( newTeam->getPlayerCount()==11|| newTeam->getGoalKeepersCount()>=1){
            m_kosherTeams->insert(nullptr,m_kosherTeams->getRoot(),newTeam);
            if (m_kosherTeams->findClosestBigger(m_kosherTeams->find(*newTeam))) {
                newTeam->setNextKosher(m_kosherTeams->findClosestBigger(m_kosherTeams->find(*newTeam))->data);
            }
            if (m_kosherTeams->findClosestSmaller(m_kosherTeams->find(*newTeam))) {
                newTeam->setPrevKosher(m_kosherTeams->findClosestSmaller(m_kosherTeams->find(*newTeam))->data);
            }
        }
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
    try{
        Team *newTeam = new Team(teamId,0);
        Node<Team>* teamNode = m_teams->find(*newTeam);
        if (!teamNode){
            delete newTeam;
            return output_t<int>(StatusType::FAILURE);
        }
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
        Player *newPlayer = new Player(playerId,0,0,0,0, false);
        Node<Player>* playerNode = teamNode->data->getPlayersById()->find(*newPlayer);
        Node<Player>* playerNodeStats = teamNode->data->getPlayersByStats()->find(*playerNode->data);
        if (!playerNode||m_playerCount==1){
            delete newTeam;
            delete newPlayer;
            return output_t<int>(StatusType::FAILURE);
        }
        Player* bigger = playerNode->data->getClosestPlayerRight();//TODO CHAGNE
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
    int size;
    if (m_kosherTeams->inpre(m_kosherTeams->getRoot())->data->getId()<minTeamId||
            m_kosherTeams->insuc(m_kosherTeams->getRoot())->data->getId()>maxTeamId) {
        return output_t<int>(StatusType::FAILURE);
    }
    try{
        findRangeTeam(minTeamId,maxTeamId,&size,arrId,arrPoints);

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
                arrPoints[index]+=arrPoints[index+diff];
            }
            diff*=2;
        }
        int winningId=arrId[0];
        delete arrId;
        delete arrPoints;
        return output_t<int>(winningId);
    } catch(...){
        delete arrId;
        delete arrPoints;
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }

}



void world_cup_t::findRangeTeam(int min, int max, int* size,int*& ids, int*& points) {
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
        while (true){
            if (current->father->right==current){
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
            if (current->father->left==current){
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
        if (currentTeam->getId()==closestToMax->data->getId()){
            break;
        }
        currentTeam=currentTeam->getNextKosher();
        *size++;
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

}

