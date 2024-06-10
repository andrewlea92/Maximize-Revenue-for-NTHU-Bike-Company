#include "./nthu_bike.h"
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
int user_num2=0; 
int benefit2=0;
#define SIZE 1010
struct bike2
{
    int type;
    int id;
    int Rental_count;
    float Rental_price;
    int station;
    int cdtime=-1;
};
struct userrequest2
{
    int id;
    int oknum=0;
    int okay[1010]={0};
    int start_time;
    int end_time;
    int start_pt;
    int end_pt;
    int bike_startpt;
    int bike_transferstartpt;
    int bike_transferendpt;
    int bike_arrive=0;
    int act_end=0;
    int money=0;
    int bikeid;
    int fulfill=0;
    bike2 ubike;
};
inline void QuickSort_user2(userrequest2 *a, const int left, const int right)
{ 
    bool bFlag = false;
    if (left < right) {
    int i = left;
    int j = right + 1;
    userrequest2 pivot = a[left];



    do 
    {
       do i++; while ( ( (a[i].start_time < pivot.start_time) || ( (a[i].start_time == pivot.start_time) && (a[i].id<pivot.id) ) )&&i<user_num2);
       do j--; while ( ( (a[j].start_time > pivot.start_time) || ( (a[j].start_time == pivot.start_time) && (a[j].id>pivot.id) ) )&&j>0);


       if (i < j)
       {
         userrequest2 tmp=a[i];
         a[i]=a[j];
         a[j]=tmp;
       }
    } while (i < j);

    userrequest2 tmp=a[left];
    a[left]=a[j];
    a[j]=tmp;
     QuickSort_user2(a, left, j - 1);
//    cout<<left<<" "<<j<<endl;
    QuickSort_user2(a, j + 1, right);
}
}
inline void QuickSort_bike2(bike2 *a, const int left, const int right)
{ 
    if (left < right) {
    int i = left;
    int j = right + 1;
    bike2 pivot = a[left];
    do {
    do i++; while (((a[i].Rental_price < pivot.Rental_price)||(a[i].Rental_price == pivot.Rental_price && a[i].id > pivot.id))&&i<=right);
    do j--; while (a[j].Rental_price > pivot.Rental_price||(a[j].Rental_price == pivot.Rental_price&&a[j].id < pivot.id));
    if (i < j)
    {
        bike2 tmp=a[i];
        a[i]=a[j];
        a[j]=tmp;
    }
    } while (i < j);
    bike2 tmp=a[left];
    a[left]=a[j];
    a[j]=tmp;
    QuickSort_bike2(a, left, j - 1);
    QuickSort_bike2(a, j + 1, right);
}
}
inline void QuickSort_cdbike2(bike2 *a, const int left, const int right)
{ 
    if (left < right) {
    int i = left;
    int j = right + 1;
    bike2 pivot = a[left];
    do {
    do i++; while (a[i].cdtime < pivot.cdtime);
    do j--; while (a[j].cdtime > pivot.cdtime);
    if (i < j)
    {
        bike2 tmp=a[i];
        a[i]=a[j];
        a[j]=tmp;
    }
    } while (i < j);
    bike2 tmp=a[left];
    a[left]=a[j];
    a[j]=tmp;
    QuickSort_cdbike2(a, left, j - 1);
    QuickSort_cdbike2(a, j + 1, right);
}
}
struct station2
{
    int id;
    int* cap;
    bike2** bk;
    int* bknum;
};
station2* stat2;
userrequest2 user_request2[100010];
bike2 all_bike2[10010];
bike2 all_bike_by_id2[10010];
bike2 unusable2[10010];    
int unsable_num2=0;
inline void swap22(bike2* a,bike2* b)
{
    bike2 tmp=*a;
    *a=*b;
    *b=tmp;
}
void advanced(string selectedCase){
    int all_bike_num=0;

    int distance[1001][1001];
    int min_distance[1001][1001];
    for(int i=0;i<1001;i++)
    {
        for(int j=0;j<1001;j++)
        {
            distance[i][j]=min_distance[i][j]=99999;
            if(i==j)
            {
                distance[i][j]=min_distance[i][j]=0;
            }
        }
    }
    int bike_init_price[1010];
    // insert your code here
    cout << "start your advance version of data structure final from here!" << endl;
    float discount;
    int count_limit;
    string line;
    //bike_info
    //_____________________________________________________________________________________________
    string path="./testcases/"+selectedCase+"/bike_info.txt";
    ifstream bike_info(path);
    int typemax=0;
    if(bike_info.is_open())
    {
        getline(bike_info,line);
        discount=stof(line);
        getline(bike_info,line);
        count_limit=stoi(line);
        while(getline(bike_info,line))
        {
            string sub1 = line.substr(0, line.find(" "));
            int id = stoi(sub1.substr(1));
            typemax++;
            string sub2 = line.substr(line.find(" ")+1, line.length());
            int init_price=stoi(sub2);
            bike_init_price[id]=init_price;
        }
        bike_info.close();
    }
    else cout<<"not open";
    //map
    //_____________________________________________________________________________________________
    path="./testcases/"+selectedCase+"/map.txt";
    ifstream mapmap(path);
    int num=0;
    if(mapmap.is_open())
    {
        while(getline(mapmap,line))
        {
            string sub1 = line.substr(0, line.find(" "));
            int station1=stoi(sub1.substr(1));
            string sub2 = line.substr(line.find(" ")+1,line.length());
            string sub3 = sub2.substr(0, line.find(" ")+1);
            int station2=stoi(sub3.substr(1));
            string sub4 = sub2.substr(line.find(" ")+1,sub2.length());
            if(station1>num) num=station1;
            if(station2>num) num=station2;
            distance[station1][station2]=distance[station2][station1]=stoi(sub4);
        }
        mapmap.close();
    }
    else cout<<"not open";
    stat2=new station2 [num+1];
    for(int i=0;i<=num;i++)
    {
        stat2[i].cap=new int[typemax+1];
        stat2[i].bk=new bike2*[typemax+1];
        stat2[i].bknum=new int[typemax+1];
        for(int j=0;j<=typemax;j++)
        {
            stat2[i].cap[j]=2;
            stat2[i].bknum[j]=0;
            stat2[i].bk[j]=new bike2[2];
        }
    }
    for (int i = 0; i<=num; i++)
    {
        for (int j = 0; j<=num; j++)
        {
            min_distance[i][j]= distance[i][j];
        }
    }
    for (int k= 0; k<=num; k++)
    {
        for (int i= 0; i<=num; i++)
        {
            for (int j= 0; j<=num; j++)
            {
                if((min_distance[i][k]+min_distance[k][j])<min_distance[i][j])
                {
                    min_distance[i][j] = min_distance[i][k] + min_distance[k][j];
                }
            }
        }
    }
    //bike
    //_____________________________________________________________________________________________
    path="./testcases/"+selectedCase+"/bike.txt";
    ifstream biketxt(path);
    if(biketxt.is_open())
    {
        
        while(getline(biketxt,line))
        {   
            int start = 0;
            int end = line.find(" ");
            int bktype = stoi(line.substr(start, end - start).substr(1));
            start = end + 1;
            end = line.find(" ", start);

            int idk = stoi(line.substr(start, end - start));
            all_bike_by_id2[idk].id=idk;
            all_bike_by_id2[idk].type=bktype;
            start = end + 1;
            end = line.find(" ", start);

            int statid = stoi(line.substr(start, end - start).substr(1));

            all_bike_by_id2[idk].station=statid;
            start = end + 1;
            end = line.find(" ", start);

            float rentalprice = stof(line.substr(start, end - start));
            all_bike_by_id2[idk].Rental_price=rentalprice;
            start = end + 1;
            end = line.find(" ", start);

            int rentalcount = stoi(line.substr(start, end - start));
            stat2[statid].id=statid;
            all_bike2[all_bike_num]=all_bike_by_id2[idk];
            all_bike_num++;
            all_bike_by_id2[idk].cdtime=0;
            all_bike_by_id2[idk].Rental_count=rentalcount;
            if(stat2[statid].bknum[bktype]>=stat2[statid].cap[bktype])
            {
                bike2* tmp=new bike2[(stat2[statid].cap[bktype])*2+1];
                for(int j=0;j<=stat2[statid].bknum[bktype];j++)
                {
                    tmp[j]=stat2[statid].bk[bktype][j];
                }
                delete[] stat2[statid].bk[bktype];
                stat2[statid].bk[bktype]=tmp;
                stat2[statid].cap[bktype]=stat2[statid].cap[bktype]*2;
            }
            stat2[statid].bk[bktype][stat2[statid].bknum[bktype]]=all_bike_by_id2[idk];
            stat2[statid].bknum[bktype]++;
        }
        biketxt.close();
    }
    else cout<<"not open";
    //user
    //_____________________________________________________________________________________________
    path="./testcases/"+selectedCase+"/user.txt";
    ifstream user(path);
    if(user.is_open())
    {
        
        while(getline(user,line))
        {   
            userrequest2 tmp;
            int start = 0;
            int end = line.find(" ");
            tmp.id = stoi(line.substr(start, end - start).substr(1));
            start = end + 1;
            end = line.find(" ", start);

            string accept = line.substr(start, end - start);
            int start1=0;
            int end1 = accept.find(',');
            while (end1 != -1) {
              tmp.okay[tmp.oknum]=stoi(accept.substr(start1, end1 - start1).substr(1));
              tmp.oknum++;
              start1 = end1 + 1;
              end1 = accept.find(',', start1);
            }
            tmp.okay[tmp.oknum]=stoi(accept.substr(start1, end1 - start1).substr(1));
            tmp.oknum++;
            start = end + 1;
            end = line.find(" ", start);

            tmp.start_time = stoi(line.substr(start, end - start));
            start = end + 1;
            end = line.find(" ", start);

            tmp.end_time = stoi(line.substr(start, end - start));
            start = end + 1;
            end = line.find(" ", start);

            tmp.start_pt = stoi(line.substr(start, end - start).substr(1));
            start = end + 1;
            end = line.find(" ", start);

            tmp.end_pt = stoi(line.substr(start, end - start).substr(1));
            start = end + 1;
            end = line.find(" ", start);
            user_request2[user_num2]=tmp;
            user_num2++;
        }   
        user.close();
    }
    else cout<<"not open";
    QuickSort_user2(user_request2,0,user_num2-1);
    for(int i=0;i<=num;i++)
    {
        station2 tmp=stat2[i];
        for(int j=0;j<typemax;j++)
        {
            if(tmp.bknum[j]!=0)
            {
                QuickSort_bike2(tmp.bk[j],0,tmp.bknum[j]-1);
                for(int p=0;p<tmp.bknum[j];p++)
                {
                    stat2[i].bk[j][p]=tmp.bk[j][p];
                }
            }
        }
    }
    for(int i=0;i<user_num2;i++)
    {
        int thenewbike=unsable_num2-1;
        while(thenewbike>0)
        {
            if(all_bike_by_id2[unusable2[thenewbike].id].cdtime>all_bike_by_id2[unusable2[thenewbike-1].id].cdtime||(all_bike_by_id2[unusable2[thenewbike].id].cdtime==all_bike_by_id2[unusable2[thenewbike-1].id].cdtime&& all_bike_by_id2[unusable2[thenewbike].id].id >all_bike_by_id2[unusable2[thenewbike-1].id].id))
            {
                swap22(&unusable2[thenewbike],&unusable2[thenewbike-1]);
                thenewbike--;
            }
            else
            {
                break;
            }
        }

        int e=unsable_num2-1;
        while(all_bike_by_id2[unusable2[e].id].cdtime<=user_request2[i].start_time&&e>=0)
        {
            if(all_bike_by_id2[unusable2[e].id].Rental_count<count_limit)
            {
                if(stat2[all_bike_by_id2[unusable2[e].id].station].bknum[all_bike_by_id2[unusable2[e].id].type]>=stat2[all_bike_by_id2[unusable2[e].id].station].cap[all_bike_by_id2[unusable2[e].id].type])
                {
                    
                    int statid=all_bike_by_id2[unusable2[e].id].station;
                    int bktype=all_bike_by_id2[unusable2[e].id].type;
                    bike2* tmp2=new bike2[(stat2[statid].cap[bktype])*2+1];
                    for(int j=0;j<=stat2[statid].bknum[bktype];j++)
                    {
                        tmp2[j]=stat2[statid].bk[bktype][j];
                    }
                    delete[] stat2[statid].bk[bktype];
                    stat2[statid].bk[bktype]=tmp2;
                    stat2[statid].cap[bktype]=stat2[statid].cap[bktype]*2;
                }

                stat2[all_bike_by_id2[unusable2[e].id].station].bk[unusable2[e].type][stat2[all_bike_by_id2[unusable2[e].id].station].bknum[unusable2[e].type]]=all_bike_by_id2[unusable2[e].id];
                stat2[all_bike_by_id2[unusable2[e].id].station].bknum[unusable2[e].type]++;
                station2 tmp=stat2[all_bike_by_id2[unusable2[e].id].station];
                QuickSort_bike2(tmp.bk[unusable2[e].type],0,tmp.bknum[unusable2[e].type]-1);
                for(int y=0;y<tmp.bknum[unusable2[e].type];y++)
                {
                    stat2[all_bike_by_id2[unusable2[e].id].station].bk[unusable2[e].type][y]=tmp.bk[unusable2[e].type][y];
                }  
            }
            unsable_num2--;
            e--;
        }
        user_request2[i].fulfill=0;
        if(min_distance[user_request2[i].start_pt][user_request2[i].end_pt]+user_request2[i].start_time>=user_request2[i].end_time)
        {
            user_request2[i].fulfill=0;
        }
        else 
        {
            station2 startstation=stat2[user_request2[i].start_pt];
            float max=-1;
            int flag=0;
            bike2 candi;
            for(int j=0;j<user_request2[i].oknum&&!flag;j++)
            {
                if(startstation.bknum[user_request2[i].okay[j]]>0) flag=1;
            }
            if(flag==0)
            {
                //advance
                int last1=-1;
                int last2=-1;
                int last3=-1;
                int last4=-1;
                int last5=-1;
                int minn=9999;
                int thenearstat=-1;
                int* used=new int[num+1];
                for(int f=0;f<num;f++)
                {
                    used[f]=0;
                }
                for(int f=0;f<num/5&&f<num;f++)
                {
                    minn=9999;
                    for(int g=0;g<num;g++)
                    {
                        if(min_distance[user_request2[i].start_pt][g]<minn&&g!=user_request2[i].start_pt)
                        {
                            //if(g!=last1&&g!=last2&&g!=last3&&g!=last4&&g!=last5)
                            if(used[g]==0)
                            {
                                minn=min_distance[user_request2[i].start_pt][g];
                                thenearstat=g;
                                //cout<<"ㄏ哈"<<last1<<endl;
                            } 
                            // else
                            // {
                            //     //cout<<"哭阿"<<endl;
                            // }
                        }
                    }
                    used[thenearstat]=1;
                    // if(last1==-1)
                    // {
                    //     last1=thenearstat;
                    //     ///cout<<"幹"<<last1<<endl;
                    // }
                    // else if(last2==-1)
                    // {
                    //     last2=thenearstat;
                    //     //cout<<"幹靠北"<<last2<<endl;
                    // }
                    // else if(last3==-1)
                    // {
                    //     ///cout<<"LLL"<<endl;
                    //     last3=thenearstat;
                    // }
                    // else if(last4==-1)
                    // {
                    //     //cout<<"TTT"<<endl;
                    //     last4=thenearstat;
                    // }
                    // else if(last5==-1)
                    // {
                    //     last5=thenearstat;
                    // }

                    //cout<<thenearstat<<endl;
                    if(min_distance[thenearstat][user_request2[i].start_pt]+min_distance[user_request2[i].start_pt][user_request2[i].end_pt]+user_request2[i].start_time<user_request2[i].end_time)
                    {
                        for(int g=0;g<user_request2[i].oknum;g++)
                        {
                            int the_type=user_request2[i].okay[g];
                            //cout<<"max: "<<max<<endl;
                            //cout<<"rental: "<<stat2[thenearstat].bk[the_type][stat2[thenearstat].bknum[the_type]-1].Rental_price<<endl;
                            if(stat2[thenearstat].bknum[user_request2[i].okay[g]]!=0&&stat2[thenearstat].bk[the_type][stat2[thenearstat].bknum[the_type]-1].Rental_price>max)
                            {
                                candi=stat2[thenearstat].bk[the_type][stat2[thenearstat].bknum[the_type]-1];
                                max=stat2[thenearstat].bk[the_type][stat2[thenearstat].bknum[the_type]-1].Rental_price;
                                //cout<<"f: "<<f<<endl;
                                
                            }
                        }
                    }
                }
                if(max>0)
                {
                    user_request2[i].bike_startpt=candi.station;
                    user_request2[i].bike_transferstartpt=candi.station;
                    user_request2[i].bike_transferendpt=user_request2[i].start_pt;
                    user_request2[i].bike_arrive=user_request2[i].start_time+min_distance[candi.station][user_request2[i].start_pt];
                    user_request2[i].fulfill=1;
                    user_request2[i].bikeid=candi.id;
                    user_request2[i].act_end=user_request2[i].start_time+min_distance[user_request2[i].start_pt][user_request2[i].end_pt]+min_distance[candi.station][user_request2[i].start_pt];
                    user_request2[i].money=(min_distance[user_request2[i].start_pt][user_request2[i].end_pt])*candi.Rental_price;
                    stat2[candi.station].bknum[candi.type]--;
                    all_bike_by_id2[candi.id].Rental_count++;
                    all_bike_by_id2[candi.id].Rental_price-=discount;
                    all_bike_by_id2[candi.id].station=user_request2[i].end_pt;
                    all_bike_by_id2[candi.id].cdtime=user_request2[i].act_end;
                    unusable2[unsable_num2]=all_bike_by_id2[candi.id];
            
                    unsable_num2++;
                    user_request2[i].ubike=candi;
                }
                else
                {
                    user_request2[i].fulfill=0;
                }
                //advance_end
            }
            else
            {
               for(int j=0;j<user_request2[i].oknum;j++)
               {
                int the_type=user_request2[i].okay[j];
                if(startstation.bknum[user_request2[i].okay[j]]!=0)
                {
                  if(startstation.bk[the_type][startstation.bknum[the_type]-1].Rental_price>max||(startstation.bk[the_type][startstation.bknum[the_type]-1].Rental_price==max&&startstation.bk[the_type][startstation.bknum[the_type]-1].id<candi.id))
                  {
                    candi=startstation.bk[the_type][startstation.bknum[the_type]-1];
                    max=startstation.bk[the_type][startstation.bknum[the_type]-1].Rental_price;
                  }  
                }
               } 
            user_request2[i].bike_arrive=0;
            user_request2[i].bike_startpt=candi.station;
            user_request2[i].fulfill=1;
            user_request2[i].bikeid=candi.id;
            user_request2[i].act_end=user_request2[i].start_time+min_distance[user_request2[i].start_pt][user_request2[i].end_pt];
            user_request2[i].money=(user_request2[i].act_end-user_request2[i].start_time)*candi.Rental_price;
            stat2[user_request2[i].start_pt].bknum[candi.type]--;
            all_bike_by_id2[candi.id].Rental_count++;
            all_bike_by_id2[candi.id].Rental_price-=discount;
            all_bike_by_id2[candi.id].station=user_request2[i].end_pt;
            all_bike_by_id2[candi.id].cdtime=user_request2[i].act_end;
            unusable2[unsable_num2]=all_bike_by_id2[candi.id];
            
            unsable_num2++;
            user_request2[i].ubike=candi;
            }
            
        }

    }  
    ofstream user_out("./result/"+selectedCase+"/user_result.txt");
    ofstream station_out("./result/"+selectedCase+"/station_status.txt");
    ofstream tranfer_out("./result/"+selectedCase+"/transfer_log.txt");
    for(int j=0;j<user_num2;j++)
    {
        for(int i=0;i<user_num2;i++)
        {
            if(user_request2[i].id==j)
            {
                if(user_request2[i].fulfill)
                {
                    benefit2+=user_request2[i].money;
                    if(user_request2[i].bike_arrive>0)
                    {
                        user_out<<"U"<<user_request2[i].id<<" "<<user_request2[i].fulfill<<" "<<user_request2[i].bikeid<<" "<<user_request2[i].bike_arrive<<" "<<user_request2[i].act_end<<" "<<user_request2[i].money<<endl;
                        tranfer_out<<user_request2[i].bikeid<<" "<<"S"<<user_request2[i].bike_startpt<<" S"<<user_request2[i].start_pt<<" "<<user_request2[i].start_time<<" "<<user_request2[i].bike_arrive<<" -1"<<endl;
                        tranfer_out<<user_request2[i].bikeid<<" "<<"S"<<user_request2[i].start_pt<<" S"<<user_request2[i].end_pt<<" "<<user_request2[i].bike_arrive<<" "<<user_request2[i].act_end<<" U"<<user_request2[i].id<<endl;
                    }
                    else
                    {
                        user_out<<"U"<<user_request2[i].id<<" "<<user_request2[i].fulfill<<" "<<user_request2[i].bikeid<<" "<<user_request2[i].start_time<<" "<<user_request2[i].act_end<<" "<<user_request2[i].money<<endl;
                        tranfer_out<<user_request2[i].bikeid<<" "<<"S"<<user_request2[i].bike_startpt<<" S"<<user_request2[i].end_pt<<" "<<user_request2[i].start_time<<" "<<user_request2[i].act_end<<" U"<<user_request2[i].id<<endl;
                    }
                }
                else
                {
                    user_out<<"U"<<user_request2[i].id<<" "<<user_request2[i].fulfill<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
                }
            }
        }
        
    }
    int idx=0;
    for(int i=0;i<10010;i++)
    {
        if(all_bike_by_id2[i].cdtime>=0)
        {
            all_bike2[idx]=all_bike_by_id2[i];
            idx++;
        }
    }
    for(int i=0;i<idx;i++)
    {
        for(int j=idx-1;j>i;j--)
        {
            if(all_bike2[j].station<all_bike2[j-1].station)
            {
                swap22(&all_bike2[j],&all_bike2[j-1]);
            }
            else if(all_bike2[j].station==all_bike2[j-1].station)
            {
                if(all_bike2[j].id<all_bike2[j-1].id)
                {
                    swap22(&all_bike2[j],&all_bike2[j-1]);
                }
            }
        }
    }
    for(int i=0;i<idx;i++)
    {
        station_out<<"S"<<all_bike2[i].station<<" "<<all_bike2[i].id<<" "<<"B"<<all_bike2[i].type<<" "<<all_bike2[i].Rental_price<<" "<<all_bike2[i].Rental_count<<endl;
    }
    //cout<<"benefit: "<<benefit2<<endl;
}
