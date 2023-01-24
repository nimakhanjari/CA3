#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proje3.h"

#define true 1
#define USERS_FILE_ADDRESS "accounts.txt"
#define POSTS_FILE_ADDRESS "posts.txt"


void main()
{
    int numberOfActivity;
    user *head = NULL;
    user *poster = NULL;
    post *headOfPosts = NULL;
    char nameOfUser[50];
    char pass[50];
    char text[100];
    char nameOfPoster[40];
    int postid;
    int check;

        printf("\nWelcome to yotitoti\n");
        printf("A place to communicate with others\n");

    while(true)
    {
    	printf("\n----------------------------------------------------\n\n");

        printf("What are you willing to do?\n");
        printf("Press:\n1 to sign up\n2 to log into an account\n3 to post something\n4 to like a post\n5 to logout\n6 to delete a post\n7 to see information of your account\n8 to find a user\n9 to print user's info in a file\n10 to print the posts details in a file\n");
        printf("\n----------------------------------------------------\n");
		scanf("%d",&numberOfActivity);

        switch(numberOfActivity)//each time the user decides what to do in the app
        {
            case 1:
                check = checkIfLoggedIn(head);
                if(check==1)
                {
                    printf("You cannot sign up while you are logged into an account\n");
                    printf("First, logout of your account so that you are able to sign up\n");
                    break;
                }
                printf("enter your username and your password:");
                scanf("%s %s",nameOfUser,pass);
                int checkUsername = checkIfUsenameIsValid(head,nameOfUser);
                if(checkUsername==0)
                {
                    break;
                }
	            head = sign_up(head,nameOfUser,pass);

                break;

            case 2:
                check = checkIfLoggedIn(head);
                if(check==1)
                {
                    printf("You are already logged into another account\n");
                    printf("First, logout of your account so that you are able to login another account\n");
                    break;
                }
                else
                {
                printf("enter the username and password of the account you wanna log into:");
                scanf("%s %s",nameOfUser,pass);
                login(head,nameOfUser,pass);

            	break;
                }

            case 3:
                if(checkIfLoggedIn(head)==0)
                {
                    printf("First log into an account");
                    break;
                }
                poster = findposter(head);
                if(poster!=NULL)
                {
                    getchar();
                    printf("\nWrite down the text you wanna post:");
                    gets(text);
                    headOfPosts = posting(headOfPosts,poster,text);
                } 
                
            	break;

            case 4:
                    ;
                    if (checkIfLoggedIn(head)==0)
                    {
                        printf("First log into an account so that you are able to like the posts you want");
                        break;
                    }
                    printf("enter the username and the post id of the post you wanna like:");
                    scanf("%s %d",nameOfUser,&postid);
				    liking(headOfPosts,postid,nameOfUser,head);
            	break;

            case 5:
                    logout(head);
	            break;

            case 6:
                if(checkIfLoggedIn(head)==0)
                {
                    printf("First log into your account");
                    break;
                }
                poster = findposter(head);
                if(findPostId(poster->username,headOfPosts)==0)
                {
                    printf("You have no post to delete");
                    break;
                }
                if(poster==NULL)
                {
                    break;
                }
            	printf("Enter the post_id of the post you wanna delete:");
                scanf("%d",&postid);
				deleteSelectedPost(headOfPosts,postid,poster);
	            break;

            case 7:
                if(checkIfLoggedIn(head)==0)
                {
                    printf("First log into an account");
                    break;
                }
                poster = findposter(head);
                if(poster!=NULL)
                {
                    showMyInfo(head,headOfPosts,poster);
                }
                break;

            case 8:
                if(checkIfLoggedIn(head)==0)
                {
                    printf("First log into an account");
                    break;
                }
                printf("Enter the username of the account you wanna see it's posts:");
                scanf("%s",nameOfUser);

                find_user(nameOfUser,headOfPosts,head);
                break;

            case 9:
                writeAccountsInfoInFile(head,headOfPosts);

                break;

            case 10:
                writePostsInfoInFile(headOfPosts);

                break;

            default:
				printf("invalid activity");
            	break;
        }
    }
}

user *sign_up(user* head,char nameOfUser[],char pass[])/*a function that enables the users who are new to yotitoti create a new account*/
{
    user *newUser = (user*)malloc(sizeof(user));

	//printf("Size:%d",strlen(nameOfUser));
	newUser->username = (char*) malloc(sizeof(char)*(strlen(nameOfUser)+1));
    strcpy(newUser->username,nameOfUser);
    newUser->password = (char*) malloc(sizeof(char)*(strlen(pass)+1));
    strcpy(newUser->password,pass);
    newUser->hasLoggedIn = 0; 
    newUser->next=NULL;

    if(head==NULL)
    {
        head=newUser;
        printf("new account created!\n");
    }
    else
    {
        user *lastNode = head;
        while(lastNode->next != NULL)
        {
            lastNode = lastNode->next;
        }

    	lastNode->next = newUser;
        printf("new account created!\n");
    }

    return head;
}

user* login(user* head,char nameOfUser[],char pass[])/*a function that enables the users log in to their accounts*/
{

    user* current = head;

    while(true)
    {
        int result1 = strcmp(current->username, nameOfUser);
        int result2 = strcmp(current->password, pass);
        //printf("\n%d \t %d\n",result1,result2);

        if(result1==0 && result2==0)
        {
            printf("welcome to your account");
            printf("\n%s",current->username);
            current->hasLoggedIn=1;
            return current;
            break;

        }
        else if(current->next==NULL)
        {
        printf("account not found");
        return NULL;
        break;
        }
        current = current->next;
    }
}

post *deleteSelectedPost(post* headOfPosts,int postid,user* poster)/*this function enables the user who has logged in their account delete a post*/
{
    post *temp1 = headOfPosts;
    post *temp2 = headOfPosts;
    
    while(temp1!=NULL)
    {        
        int result = strcmp(poster->username,temp1->username);
        
        if(temp1->post_id==postid && result==0)
        {

            if(temp1==temp2)
            {
                /* this condition will run if
                the record that we need to delete is the first node
                of the linked list */
                headOfPosts = headOfPosts->next;
                free(temp1);
                printf("The selected post was deleted");
            }
            else
            {
                /* temp1 is the node we need to delete
                 temp2 is the node previous to temp1 */
                temp2->next = temp1->next;
                free(temp1); 
                printf("The selected post was deleted");
            }
            
            return headOfPosts;
        }
        temp2 = temp1;
        temp1 = temp1->next;
    }
    printf("Post was not found");
    return headOfPosts;
}

int checkIfLoggedIn(user* head)/*this helps the program to check if the user is already logged in an account or not*/
{
    user* current=head;
    while(current!=NULL)
    {
        if(current->hasLoggedIn==1)
        {
            return 1;
            break;
        }
        current = current->next;

    }
    return 0;
}

user* logout(user* head)/*a function that enables the users who have logged in, log out of their account*/
{
    user* current=head;
        while(current!=NULL)
        {
            if(current->hasLoggedIn==1)
            {
                current->hasLoggedIn=0;
                printf("you are logged out of your account");
                break;
            }
            current = current->next;

        }
        if(current==NULL)
        {
            printf("You were not logged into any account what are you logging out of?");
        }
        return current;
}

post *posting(post* headOfPosts,user* poster,char text[])/*By using this function, the user can post anything he wants*/
{    
    post *newPost=(post*) malloc(sizeof(post));

    newPost->next=NULL;

    newPost->post_text = (char*) malloc(sizeof(char)*(strlen(text)+1));
    strcpy(newPost->post_text,text);

    newPost->username = (char*) malloc(sizeof(char)*(strlen(poster->username)+1));
    strcpy(newPost->username,poster->username);

    newPost->post_id = findPostId(newPost->username,headOfPosts)+1;

    newPost->like=0;

    if(headOfPosts==NULL)
    {
        headOfPosts=newPost;
        /*printf("added at the beginning\n");*/
    }
    else
    {
        post *lastNode = headOfPosts;
        while(lastNode->next != NULL)
        {
            lastNode = lastNode->next;
        }

    	lastNode->next = newPost;
        /*printf("added later\n");*/
    }

    printf("Your new post:%s\nUser: %s\nPost_id: %d",newPost->post_text,newPost->username,newPost->post_id);

    return headOfPosts;
}

void liking(post* headOfPosts,int postid,char nameOfUser[],user* head)/*this function enables the users like the post they want*/
{
    post* currentpost=headOfPosts;
    user* liker = findposter(head);

    while(currentpost!=NULL)
    {
        int result=strcmp(currentpost->username,nameOfUser);
        if(result==0 && currentpost->post_id==postid)
        {
            currentpost->like++;
            printf("\n%s liked this post",liker->username);
            printf("\nLIKES : %d",currentpost->like);

            return;
        }
        currentpost=currentpost->next;
    }

    printf("Post was not found");
    
    return;
}

user* findposter(user* head)/*this function finds the user who is logged in( who can also post texts)*/
{
    user* current = head;
   
    while(current!=NULL)
    {
        if(current->hasLoggedIn==1)
        {
            return current;
        }
        current = current->next;
    }
    if(current==NULL)
    {
        printf("\nFirst log into an account\n");
        return NULL;
    }
}

void showMyInfo(user* head,post* headOfPosts,user* poster)/*Each user can view their own information using this function*/
{
    user* myAccount = poster;
    printf("\nYour username:%s \nYour password:%s\n",myAccount->username,myAccount->password);
    printf("Your posts:\n");
    int numberOfPosts = findPostId(myAccount->username,headOfPosts);
    if(numberOfPosts==0)
    {
        printf("NO POSTS");
        return;
    }
    printPosts(headOfPosts,myAccount->username);
    
}

void printPosts(post* headOfPosts,char nameOfUser[])/*this function prints all the details about each post of the user searched for*/
{
    post* currentPost = headOfPosts;
    while(currentPost !=NULL)
    {
        int result = strcmp(currentPost->username,nameOfUser);
        if(result==0)
        {
            printf("\nPost:%s\tLikes:%d\t\tpost id:%d\n",currentPost->post_text,currentPost->like,currentPost->post_id);
        }
        currentPost = currentPost->next;
    }
}

int findPostId(char nameOfUser[],post* headOfPosts)/*this function gives a specified id to each post the user posts*/
{   
    int numberOfPosts=0;
    post* currentpost = headOfPosts;
    while(currentpost!=NULL)
    {
        int result = strcmp(currentpost->username,nameOfUser);
        if(result==0)
        {
        numberOfPosts++;
        }
        currentpost = currentpost->next;
    }
    return numberOfPosts;
}

void find_user(char nameOfUser[],post* headOfPosts,user* head)/*By using this function the user can see all the posts from the user he has searched for*/
{
    user* current = head;
    while(current!=NULL)
    {
        int result = strcmp(current->username,nameOfUser);

        if(result==0)
        {
            printf("\nUsername:%s",current->username);
            printPosts(headOfPosts,current->username);
            return;
        }

        current = current->next;
  
    }

    printf("The user you searched for was not found");
    return;

}

int checkIfUsenameIsValid(user*head,char nameOfUser[])/*this function checks if the username entered in the sign up section is already in use or not*/
{
    user* current = head;
    while(current!=NULL)
    {
        if(strcmp(current->username,nameOfUser)==0)
        {
            printf("username already taken");
            return 0;
        }
        current = current->next;
    }
    return 1;
}


void writeAccountsInfoInFile(user* head, post* headOfPosts)/*the function that prints all the details about each user shared in a file named "accounts"*/
{
    FILE* file1 = fopen(USERS_FILE_ADDRESS,"w");
    user* current = head;

    while (current!=NULL)
    {    
        int numberOfPosts = findPostId(current->username,headOfPosts);
        fprintf(file1,"%s %s %d\n",current->username,current->password,numberOfPosts);
        current = current->next;
    }
    printf("Users info printed in your file");
    fclose(file1);
    return;
}

void writePostsInfoInFile(post* headOfPosts)/*the function that prints all the details about each post shared in a file named "posts"*/
{
    FILE* file2 = fopen(POSTS_FILE_ADDRESS,"w");
    post* currentpost = headOfPosts;

    while (currentpost!=NULL)
    {    
        fprintf(file2,"%s %s %d\n",currentpost->post_text,currentpost->username,currentpost->like);
        currentpost = currentpost->next;
    }
    printf("Posts details printed in your file");
    fclose(file2);
    return;
}
