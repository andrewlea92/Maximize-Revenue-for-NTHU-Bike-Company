#include "./nthu_bike.h"
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
int user_num=0; 
int benefit=0;
#define SIZE 1010
struct bike
{
    int type;
    int id;
    int Rental_count;
    float Rental_price;
    int station;
    int cdtime=-1;
};
struct userrequest
{
    int id;
    int oknum=0;
    int okay[1010]={0};
    int start_time;
    int end_time;
    int start_pt;
    int end_pt;
    int act_end=0;
    int money=0;
    int bikeid;
    int fulfill=0;
    bike ubike;
};
inline void QuickSort_user(userrequest *a, const int left, const int right)
{ 
    bool bFlag = false;
    if (left < right) {
    int i = left;
    int j = right + 1;
    userrequest pivot = a[left];

    // if (left==997)
    //   bFlag = true;

    do 
    {
       do i++; while ( ( (a[i].start_time < pivot.start_time) || ( (a[i].start_time == pivot.start_time) && (a[i].id<pivot.id) ) )&&i<user_num);
       do j--; while ( ( (a[j].start_time > pivot.start_time) || ( (a[j].start_time == pivot.start_time) && (a[j].id>pivot.id) ) )&&j>0);
    //    if (bFlag)
    //        cout<<i<<" "<<j<<endl;

       if (i < j)
       {
         userrequest tmp=a[i];
         a[i]=a[j];
         a[j]=tmp;
       }
    } while (i < j);

    userrequest tmp=a[left];
    a[left]=a[j];
    a[j]=tmp;
     QuickSort_user(a, left, j - 1);
//    cout<<left<<" "<<j<<endl;
    QuickSort_user(a, j + 1, right);
}
}
inline void QuickSort_bike(bike *a, const int left, const int right)
{ 
    if (left < right) {
    int i = left;
    int j = right + 1;
    bike pivot = a[left];
    do {
    do i++; while (((a[i].Rental_price < pivot.Rental_price)||(a[i].Rental_price == pivot.Rental_price && a[i].id > pivot.id))&&i<=right);
    do j--; while (a[j].Rental_price > pivot.Rental_price||(a[j].Rental_price == pivot.Rental_price&&a[j].id < pivot.id));
    if (i < j)
    {
        bike tmp=a[i];
        a[i]=a[j];
        a[j]=tmp;
    }
    } while (i < j);
    bike tmp=a[left];
    a[left]=a[j];
    a[j]=tmp;
    QuickSort_bike(a, left, j - 1);
    QuickSort_bike(a, j + 1, right);
}
}
inline void QuickSort_cdbike(bike *a, const int left, const int right)
{ 
    if (left < right) {
    int i = left;
    int j = right + 1;
    bike pivot = a[left];
    do {
    do i++; while (a[i].cdtime < pivot.cdtime);
    do j--; while (a[j].cdtime > pivot.cdtime);
    if (i < j)
    {
        bike tmp=a[i];
        a[i]=a[j];
        a[j]=tmp;
    }
    } while (i < j);
    bike tmp=a[left];
    a[left]=a[j];
    a[j]=tmp;
    QuickSort_cdbike(a, left, j - 1);
    QuickSort_cdbike(a, j + 1, right);
}
}
struct station
{
    int id;
    int* cap;
    //bike bk[200][32];
    //int bknum[200]={0};
    bike** bk;
    int* bknum;
};
station* stat;
userrequest user_request[100010];
bike all_bike[10010];
bike all_bike_by_id[10010];
bike unusable[10010];    
int unsable_num=0;
inline void swap2(bike* a,bike* b)
{
    bike tmp=*a;
    *a=*b;
    *b=tmp;
}
void basic(string selectedCase){
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
    cout << "start your basic version of data structure final from here!" << endl;
    float discount;
    int count_limit;
    string line;
    //bike_info
    //_____________________________________________________________________________________________
    string path="./testcases/"+selectedCase+"/bike_info.txt";
    ifstream bike_info(path);
    int typemax=0;
    //int stationmax=0;
    if(bike_info.is_open())
    {
        getline(bike_info,line);
        discount=stof(line);
        //cout<<"discount:"<<discount<<endl;
        getline(bike_info,line);
        count_limit=stoi(line);
        //cout<<"count_limit:"<<count_limit<<endl;
        while(getline(bike_info,line))
        {
            string sub1 = line.substr(0, line.find(" "));
            int id = stoi(sub1.substr(1));
            typemax++;
            //cout<<id<<endl;
            string sub2 = line.substr(line.find(" ")+1, line.length());
            int init_price=stoi(sub2);
            //cout<<init_price<<endl;
            //cout<<line<<endl;
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
            //cout<<sub1<<endl;
            int station1=stoi(sub1.substr(1));
            //cout<<station1<<endl;
            string sub2 = line.substr(line.find(" ")+1,line.length());
            string sub3 = sub2.substr(0, line.find(" ")+1);
            //cout<<sub3<<endl;
            int station2=stoi(sub3.substr(1));
            //cout<<station2<<endl;
            //cout<<sub3<<endl;
            string sub4 = sub2.substr(line.find(" ")+1,sub2.length());
            //cout<<sub4<<endl;
            if(station1>num) num=station1;
            if(station2>num) num=station2;
            distance[station1][station2]=distance[station2][station1]=stoi(sub4);
            //cout<<line<<endl;
        }
        mapmap.close();
    }
    else cout<<"not open";
    stat=new station[num+1];
    for(int i=0;i<=num;i++)
    {
        stat[i].cap=new int[typemax+1];
        stat[i].bk=new bike*[typemax+1];
        stat[i].bknum=new int[typemax+1];
        for(int j=0;j<=typemax;j++)
        {
            stat[i].cap[j]=2;
            stat[i].bknum[j]=0;
            stat[i].bk[j]=new bike[2];
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
    // for (int i = 0; i<=num; i++)
    // {
    //     for (int j = 0; j<=num; j++)
    //     {
    //         cout<<min_distance[i][j]<<" ";
    //     }
    //     cout<<endl;
    //     cout<<endl;
    // }
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
            //cout << line.substr(start, end - start) << endl;
            int bktype = stoi(line.substr(start, end - start).substr(1));
            start = end + 1;
            end = line.find(" ", start);

            //cout << line.substr(start, end - start) << endl;
            int idk = stoi(line.substr(start, end - start));
            all_bike_by_id[idk].id=idk;
            all_bike_by_id[idk].type=bktype;
            start = end + 1;
            end = line.find(" ", start);

            //cout << line.substr(start, end - start) << endl;
            int statid = stoi(line.substr(start, end - start).substr(1));
            // (stat[statid].bk[bktype][stat[statid].bknum[bktype]]).id=id;
            // (stat[statid].bk[bktype][stat[statid].bknum[bktype]]).cdtime=0;
            // (stat[statid].bk[bktype][stat[statid].bknum[bktype]]).Rental_count=rentalcount;
            // (stat[statid].bk[bktype][stat[statid].bknum[bktype]]).station=statid;
            // (stat[statid].bk[bktype][stat[statid].bknum[bktype]]).type=bktype;
            // stat[statid].bknum[bktype]++;
            // tmp.station=statid;
            all_bike_by_id[idk].station=statid;
            start = end + 1;
            end = line.find(" ", start);

            //cout << line.substr(start, end - start) << endl;
            float rentalprice = stof(line.substr(start, end - start));
            // tmp.Rental_price=rentalprice;
            all_bike_by_id[idk].Rental_price=rentalprice;
            start = end + 1;
            end = line.find(" ", start);

            //cout << line.substr(start, end - start) << endl;
            int rentalcount = stoi(line.substr(start, end - start));
            // tmp.Rental_count=rentalcount;
            // tmp.cdtime=0;
            stat[statid].id=statid;
            all_bike[all_bike_num]=all_bike_by_id[idk];
            all_bike_num++;
            all_bike_by_id[idk].cdtime=0;
            all_bike_by_id[idk].Rental_count=rentalcount;
            if(stat[statid].bknum[bktype]>=stat[statid].cap[bktype])
            {
                bike* tmp=new bike[(stat[statid].cap[bktype])*2+1];
                for(int j=0;j<=stat[statid].bknum[bktype];j++)
                {
                    tmp[j]=stat[statid].bk[bktype][j];
                }
                delete[] stat[statid].bk[bktype];
                stat[statid].bk[bktype]=tmp;
                stat[statid].cap[bktype]=stat[statid].cap[bktype]*2;
            }
            stat[statid].bk[bktype][stat[statid].bknum[bktype]]=all_bike_by_id[idk];
            stat[statid].bknum[bktype]++;
        // cout << str.substr(start, end - start);
            //cout<< stat[statid].bk[bktype][stat[statid].bknum[bktype]-1].id<<endl;
        }
        biketxt.close();
        // for(int i=0;i<stat[1].bknum[0];i++)
        // {
        //     cout<<stat[1].bk[0][i].id<<" ";
        // }
        // cout<<endl;
        // for(int i=0;i<stat[1].bknum[1];i++)
        // {
        //     cout<<stat[1].bk[1][i].id<<" ";
        // }
        // cout<<endl;
        // for(int i=0;i<stat[1].bknum[2];i++)
        // {
        //     cout<<stat[1].bk[2][i].id<<" ";
        // }
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
            userrequest tmp;
            int start = 0;
            int end = line.find(" ");
            tmp.id = stoi(line.substr(start, end - start).substr(1));
            start = end + 1;
            end = line.find(" ", start);

            string accept = line.substr(start, end - start);
            int start1=0;
            int end1 = accept.find(',');
            while (end1 != -1) {
            //    cout << str.substr(start, end - start) << endl;
            //   tmp.okay[stoi(accept.substr(start1, end1 - start1).substr(1))]=1;
              //tmp.okay[tmp.oknum]=1;
              tmp.okay[tmp.oknum]=stoi(accept.substr(start1, end1 - start1).substr(1));
              tmp.oknum++;
              start1 = end1 + 1;
              end1 = accept.find(',', start1);
            }
            // tmp.okay[stoi(accept.substr(start1, end1 - start1).substr(1))]=1;
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
            // user_request[tmp.id]=tmp;
            user_request[user_num]=tmp;
            user_num++;
            // cout<<user_request[tmp.id].oknum<<endl;
            // cout<<user_request[49].okay[0]<<" "<<user_request[49].okay[1]<<" "<< user_request[49].okay[2]<<endl;
            // cout<<user_request[49].start_pt<<" "<<user_request[49].end_pt<<" "<<user_request[49].start_time<<" "<<user_request[49].end_time<<endl;
        }   
        user.close();
    }
    else cout<<"not open";
    // for(int i=0;i<user_num;i++)
    // {
    //     cout<<i<<": "<<user_request[i].id<<","<<user_request[i].start_time<<endl;
    // }
    // for(int i=0;i<user_num;i++)
    // {
    //     cout<<user_request[i].id<<" "<<user_request[i].start_time<<endl;
    // }
    // cout<<user_num<<endl;
     ///cout<<"hi"<<endl;
    QuickSort_user(user_request,0,user_num-1);
     //cout<<"hi"<<endl;
    //_____________________________________________________________________________________________
    // for(int i=0;i<user_num;i++)
    // {
    //     cout<<i<<": "<<user_request[i].id<<","<<user_request[i].oknum<<endl;
    // }
    for(int i=0;i<=num;i++)
    {
        //cout<<i<<endl;
        station tmp=stat[i];
        // for(int d=0;d<100;d++)
        // {
        //     for(int m=0;m<stat[i].bknum[d];m++)
        //     {
        //         cout<<"station id: "<<stat[i].id<<" biketype: "<<d<<" bikeid: "<<stat[i].bk[d][m].id<<endl;
        //     }
        // }
        for(int j=0;j<typemax;j++)
        {
            //cout<<"    "<<j<<endl;
            // cout<<"+++++"<<tmp.bknum[j]<<endl;
            // for(int y=0;y<tmp.bknum[j];y++)
            // {
            //     cout<<"bike"<<tmp.bk[j][y].id<<"price"<< tmp.bk[j][y].Rental_price<< endl;
            // }
            if(tmp.bknum[j]!=0)
            {
                QuickSort_bike(tmp.bk[j],0,tmp.bknum[j]-1);
                for(int p=0;p<tmp.bknum[j];p++)
                {
                    stat[i].bk[j][p]=tmp.bk[j][p];
                }
            }
            //cout<<"end:"<<j<<endl;
        }
        // for(int d=0;d<100;d++)
        // {
        //     for(int m=0;m<tmp.bknum[d];m++)
        //     {
        //         // cout<<d<<" "<<stat[i].bknum[d]<<"bikes"<<endl;
        //         cout<<"*****station id: "<<tmp.id<<" biketype: "<<d<<" bikeid: "<<tmp.bk[d][m].id<<endl;
        //     }
        // }
        
    }
    // for(int i=0;i<100;i++)
    // {
    //     cout<<"station"<<i<<endl;
    //     for(int j=0;j<100;j++)
    //     {
    //         if(stat[i].bknum[j]!=0)
    //         {
    //             cout<<"type"<<j<<endl;
    //             for(int k=0;k<stat[i].bknum[j];k++)
    //             {
    //                 cout<<"id: "<<stat[i].bk[j][k].id<<" rental: "<<stat[i].bk[j][k].Rental_price<<endl;
    //             }
    //         }
    //     }
    // }
    // for(int i=0;i<30;i++)
    // {
    //     cout<<all_bike_by_id[i].id<<" id:stat "<<all_bike_by_id[i].station<<endl;
    //     cout<<all_bike_by_id[i].Rental_price<<" price"<<endl;
    // }
    for(int i=0;i<user_num;i++)
    {
        //cout<<i<<endl;
        // for(int j=0;j<100;j++)
        // {
        //     for(int k=0;k<100;k++)
        //     {
        //         if(stat[j].bknum[k]!=0)
        //         {
        //             for(int h=0;h<stat[j].bknum[k];h++)
        //             {
        //                 cout<<"______station "<<stat[j].bk[k][h].station<<" ______type "<<stat[j].bk[k][h].type<<" _____id "<<stat[j].bk[k][h].id<<endl;
        //             }
        //             cout<<endl;
        //         }
        //     }
        // }
        // cout<<endl;
        // if(user_request[i].start_time==1009)
        // {
        //     cout<<"start: "<<user_request[i].start_pt<<" end: "<<user_request[i].end_pt<<endl;
        //     cout<<"XDD :"<<user_request[i].id<<endl;
        // }
        // if(user_request[i].start_time>1040&&user_request[i].start_time<1050)
        // {
        //     cout<<":) :"<<user_request[i].id<<endl;
        //     for(int g=0;g<10;g++)
        //     {
        //         cout<<"station"<<g<<endl;
        //         for(int j=0;j<100;j++)
        //         {
        //         if(stat[g].bknum[j]!=0)
        //          {
        //             cout<<"type"<<j<<endl;
        //             for(int k=0;k<stat[g].bknum[j];k++)
        //             {
        //              cout<<"id: "<<stat[g].bk[j][k].id<<" rental: "<<stat[g].bk[j][k].Rental_price<<endl;
        //             }
        //          }
        //         }
        //     }
            
        // }

        // if(unsable_num!=0)
        // {
        //     for(int r=0;r<unsable_num;r++)
        //     {
        //         for(int h=0;h<(unsable_num-r-1);h++)
        //         {
        //             if(all_bike_by_id[unusable[h].id].cdtime<all_bike_by_id[unusable[h+1].id].cdtime||(all_bike_by_id[unusable[h].id].cdtime==all_bike_by_id[unusable[h+1].id].cdtime&&all_bike_by_id[unusable[h].id].id<all_bike_by_id[unusable[h+1].id].id))
        //             {
        //                 swap2(&unusable[h],&unusable[h+1]);
        //             }
        //         }
        //     }

        // }
        int thenewbike=unsable_num-1;
        while(thenewbike>0)
        {
            if(all_bike_by_id[unusable[thenewbike].id].cdtime>all_bike_by_id[unusable[thenewbike-1].id].cdtime||(all_bike_by_id[unusable[thenewbike].id].cdtime==all_bike_by_id[unusable[thenewbike-1].id].cdtime&& all_bike_by_id[unusable[thenewbike].id].id >all_bike_by_id[unusable[thenewbike-1].id].id))
            {
                swap2(&unusable[thenewbike],&unusable[thenewbike-1]);
                thenewbike--;
            }
            else
            {
                break;
            }
        }

        int e=unsable_num-1;
        while(all_bike_by_id[unusable[e].id].cdtime<=user_request[i].start_time&&e>=0)
        {
            if(all_bike_by_id[unusable[e].id].Rental_count<count_limit)
            {
                if(stat[all_bike_by_id[unusable[e].id].station].bknum[all_bike_by_id[unusable[e].id].type]>=stat[all_bike_by_id[unusable[e].id].station].cap[all_bike_by_id[unusable[e].id].type])
                {
                    
                    int statid=all_bike_by_id[unusable[e].id].station;
                    int bktype=all_bike_by_id[unusable[e].id].type;
                    bike* tmp2=new bike[(stat[statid].cap[bktype])*2+1];
                    for(int j=0;j<=stat[statid].bknum[bktype];j++)
                    {
                        tmp2[j]=stat[statid].bk[bktype][j];
                    }
                    delete[] stat[statid].bk[bktype];
                    stat[statid].bk[bktype]=tmp2;
                    stat[statid].cap[bktype]=stat[statid].cap[bktype]*2;
                }

                stat[all_bike_by_id[unusable[e].id].station].bk[unusable[e].type][stat[all_bike_by_id[unusable[e].id].station].bknum[unusable[e].type]]=all_bike_by_id[unusable[e].id];
                stat[all_bike_by_id[unusable[e].id].station].bknum[unusable[e].type]++;
                station tmp=stat[all_bike_by_id[unusable[e].id].station];
                QuickSort_bike(tmp.bk[unusable[e].type],0,tmp.bknum[unusable[e].type]-1);
                for(int y=0;y<tmp.bknum[unusable[e].type];y++)
                {
                    stat[all_bike_by_id[unusable[e].id].station].bk[unusable[e].type][y]=tmp.bk[unusable[e].type][y];
                }  
            }
            unsable_num--;
            e--;
        }
        user_request[i].fulfill=0;
        if(min_distance[user_request[i].start_pt][user_request[i].end_pt]+user_request[i].start_time>=user_request[i].end_time)
        {
            user_request[i].fulfill=0;
        }
        else 
        {
            station startstation=stat[user_request[i].start_pt];
            float max=-1;
            int flag=0;
            bike candi;
            for(int j=0;j<user_request[i].oknum&&!flag;j++)
            {
                if(startstation.bknum[user_request[i].okay[j]]>0) flag=1;
            }
            if(flag==0)
            {
                user_request[i].fulfill=0;
            }
            else
            {
               for(int j=0;j<user_request[i].oknum;j++)
               {
                int the_type=user_request[i].okay[j];
                if(startstation.bknum[user_request[i].okay[j]]!=0)
                {
                  if(startstation.bk[the_type][startstation.bknum[the_type]-1].Rental_price>max||(startstation.bk[the_type][startstation.bknum[the_type]-1].Rental_price==max&&startstation.bk[the_type][startstation.bknum[the_type]-1].id<candi.id))
                  {
                    candi=startstation.bk[the_type][startstation.bknum[the_type]-1];
                    max=startstation.bk[the_type][startstation.bknum[the_type]-1].Rental_price;
                  }  
                }
               } 
            user_request[i].fulfill=1;
            user_request[i].bikeid=candi.id;
            user_request[i].act_end=user_request[i].start_time+min_distance[user_request[i].start_pt][user_request[i].end_pt];
            user_request[i].money=(user_request[i].act_end-user_request[i].start_time)*candi.Rental_price;
            stat[user_request[i].start_pt].bknum[candi.type]--;
            all_bike_by_id[candi.id].Rental_count++;
            all_bike_by_id[candi.id].Rental_price-=discount;
            all_bike_by_id[candi.id].station=user_request[i].end_pt;
            all_bike_by_id[candi.id].cdtime=user_request[i].act_end;
            unusable[unsable_num]=all_bike_by_id[candi.id]; 
            unsable_num++;
            user_request[i].ubike=candi;
            }
            
        }
        // cout<<user_request[i].fulfill<<endl;
         //if(user_request[i].fulfill==1)
         //{
        //     cout<<"id: "<<user_request[i].id<<" startpt: "<<user_request[i].start_pt<<" bikeid: "<<user_request[i].bikeid<<" actend: "<<user_request[i].act_end<<" money: "<<user_request[i].money<<endl;
        //     cout<<"bikeid: "<<user_request[i].ubike.id<<"price: "<<user_request[i].ubike.Rental_price<<" rentalct: "<<user_request[i].ubike.Rental_count<<endl;
        //     cout<<"endpt: "<<user_request[i].end_pt<<" arrive: "<<user_request[i].act_end<<endl;
        //string a;
        ///cin>>a;
        //cout<<"U"<<user_request[i].id<<" "<<user_request[i].fulfill<<" "<<user_request[i].bikeid<<" "<<user_request[i].start_time<<" "<<user_request[i].act_end<<" "<<user_request[i].money<<endl;
        // if(user_request[i].id==10166)
        // {
        //     cout<<" num:"<<user_request[i].oknum<<endl;
        //     for(int u=0;u<user_request[i].oknum;u++)
        //     {
        //         cout<<"    ok: "<<user_request[i].okay[u]<<endl;
        //     }
        // }
        // for(int g=0;g<100;g++)
        // {
        //     if(stat[10].bknum[g]>0)
        //     {
        //         cout<<"type "<<g<<endl;
        //         for(int u=0;u<stat[10].bknum[g];u++)
        //         {
        //             cout<<"    bikeid: "<<stat[10].bk[g][u].id<<" rental_price: "<<stat[10].bk[g][u].Rental_price<<endl;
        //         }
        //     }
        // }
        //     cout<<endl;
        // }
        // else{
        //     cout<<"fail_user_request[i].id: "<<user_request[i].id<<"  at :"<<user_request[i].start_time<< endl;
        //                         }
        // if(user_request[i].bikeid==1141)
        // {
        //     if(user_request[i].fulfill)
        //     {
        //          cout<<"U"<<user_request[i].id<<" "<<user_request[i].fulfill<<" "<<user_request[i].bikeid<<" "<<user_request[i].start_time<<" "<<user_request[i].act_end<<" "<<user_request[i].money<<endl;
        //     }
        //     else
        //     {
        //         cout<<"U"<<user_request[i].id<<" "<<user_request[i].fulfill<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
        //     }
        // }
        
        
    }
    // for(int i=0;i<50;i++)
    // {
    //     cout<<"station: "<<i<<endl;
    //     for(int j=0;j<10;j++)
    //     {
    //         if(stat[i].bknum[j]!=0)
    //         {
    //             cout<<"type: "<<j<<endl;
    //             for(int k=0;k<stat[i].bknum[j];k++)
    //             {
    //                 cout<< "id: "<<stat[i].bk[j][k].id<<" price: "<<stat[i].bk[j][k].Rental_price<<" count: "<<stat[i].bk[j][k].Rental_count<<endl;
    //             }
    //             cout<<endl;
    //         }
    //     }
    // }
    
    
    ofstream user_out("./result/"+selectedCase+"/user_result.txt");
    ofstream station_out("./result/"+selectedCase+"/station_status.txt");
    ofstream tranfer_out("./result/"+selectedCase+"/transfer_log.txt");
    for(int j=0;j<user_num;j++)
    {
        for(int i=0;i<user_num;i++)
        {
            if(user_request[i].id==j)
            {
                if(user_request[i].fulfill)
                {
                    benefit+=user_request[i].money;
                    user_out<<"U"<<user_request[i].id<<" "<<user_request[i].fulfill<<" "<<user_request[i].bikeid<<" "<<user_request[i].start_time<<" "<<user_request[i].act_end<<" "<<user_request[i].money<<endl;
                    tranfer_out<<user_request[i].bikeid<<" "<<"S"<<user_request[i].start_pt<<" S"<<user_request[i].end_pt<<" "<<user_request[i].start_time<<" "<<user_request[i].act_end<<" U"<<user_request[i].id<<endl;
                }
                else
                {
                    user_out<<"U"<<user_request[i].id<<" "<<user_request[i].fulfill<<" "<<"0"<<" "<<"0"<<" "<<"0"<<" "<<"0"<<endl;
                }
                // if(user_request[i].bikeid==80)
                // {
                //     cout<<"U"<<user_request[i].id<<" "<<user_request[i].fulfill<<" "<<user_request[i].bikeid<<" "<<user_request[i].start_time<<" "<<user_request[i].act_end<<" "<<user_request[i].money<<endl;
                // }
            }
        }
        
    }
    int idx=0;
    for(int i=0;i<10010;i++)
    {
        if(all_bike_by_id[i].cdtime>=0)
        {
            all_bike[idx]=all_bike_by_id[i];
            idx++;
        }
    }
    for(int i=0;i<idx;i++)
    {
        for(int j=idx-1;j>i;j--)
        {
            if(all_bike[j].station<all_bike[j-1].station)
            {
                swap2(&all_bike[j],&all_bike[j-1]);
            }
            else if(all_bike[j].station==all_bike[j-1].station)
            {
                if(all_bike[j].id<all_bike[j-1].id)
                {
                    swap2(&all_bike[j],&all_bike[j-1]);
                }
            }
        }
    }
    //cout<<"+++++++++++++++station_status+++++++++++++++++++"<<endl;
    for(int i=0;i<idx;i++)
    {
        station_out<<"S"<<all_bike[i].station<<" "<<all_bike[i].id<<" "<<"B"<<all_bike[i].type<<" "<<all_bike[i].Rental_price<<" "<<all_bike[i].Rental_count<<endl;
    }
    //cout<<"benefit: "<<benefit<<endl;
}