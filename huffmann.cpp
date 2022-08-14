#include <stdio.h>
#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
/*Just run this file in terminal using g++ huffmann.cpp and then a.exe  you will get that compress file*/
using namespace std;
struct node{
    char val;
    int data;
    struct node* left;
    struct node* right;
    node(char ch,int freq){
        val = ch;
        data = freq;
        left = NULL;
        right = NULL;
    }
}*root,*tmp;
struct compare{
    bool operator()(node* n1,node* n2)
    {
        return n1->data > n2->data;
    }
};
node* merge(node *t1,node *t2){
    node * res = new node('$',0);
    res->data = t1->data + t2->data;
    res->left = t1;
    res->right = t2;
    return res;
}
node* huffmann_encoding(vector<pair<char,int>> chars){
    priority_queue<node*,vector<node*>,compare> mn_hp;
    for(int i=0;i<chars.size();i++){
        tmp = new node(chars[i].first,chars[i].second);
        mn_hp.push(tmp);
    }
    //cout<<mn_hp.top()->data;
    while(mn_hp.size()>1){
        struct node * t1 = mn_hp.top();
        mn_hp.pop();
        struct node * t2 = mn_hp.top();
        mn_hp.pop();
        struct node * nd = merge(t1,t2);
        mn_hp.push(nd);
    }
    return mn_hp.top();
}
void get_codes(node * root,string codes,unordered_map<char,string> &mp,unordered_map<string,char> &mp_rev){
    if(root->val != '$'){
        mp.insert(make_pair(root->val, codes));
        mp_rev.insert(make_pair(codes,root->val));
        //cds.push_back(make_pair(root->val, codes));
        return;
    }
    get_codes(root->left,codes+"0",mp,mp_rev);
    get_codes(root->right,codes+"1",mp,mp_rev);
}
void decode(node * root,string encoded){
    node * tmp = root;
    while(tmp && encoded.length()){
        if(encoded[0] == '1'){
            tmp = tmp->right;
        }
        else{
            tmp = tmp->left;
        }
        encoded = encoded.substr(1);
    }
    if(tmp){
        cout<<tmp->val<<endl;
        return;
    }
    cout<<"No such character!!";
}
void decode_string(string s,unordered_map<string,char>mp_rev){
    int i = 0;
    string decoded = "";
    string tmp = "";
    while(i < s.length()){
        tmp += s[i];
        if(mp_rev.find(tmp) != mp_rev.end()){
            //cout << tmp << endl;
            decoded += mp_rev[tmp];
            tmp = "";
        }
        i++;
    }
    cout<<decoded<<endl;
}
string decode_string_just_tree(node* root,string s){
    int i = 0;
    string decoded = "";
    struct node *tmp = root;
    while(i < s.length()){
        if(s[i] == '0'){
            tmp = tmp->left;
        }
        else{
            tmp = tmp->right;
        }
        if(tmp->val != '$'){  //we have reached a decoded char
            decoded += tmp->val;
            tmp = root;
        }
        i++;
    }
    return decoded;
}
vector<pair<char,int>> get_freqs(string s){
    unordered_map<char,int> mp;
    for(int i=0;i<s.length();i++){
        mp[s[i]]++;
    }
    vector<pair<char,int>> res;
    for(auto i:mp){
        res.push_back(make_pair(i.first,i.second));
    }
    return res;
}
string dec_bin(int val){
    string res = "";
    int num = 0;
    while(num < 8){
        if(val){
            int rem = val % 2;
            val = val / 2;
            res = to_string(rem)+res;
        }
        else{
            res = "0"+res;
        }
        num++;
    }
    return res;
}
int bin_dec(string val){
    int res = 0;
    int i = 0;
    while(i < 8){
        if(val[7-i] == '1'){
            res += pow(2,i);
        }
        i++;
    }
    return res;
}
string padding(string enc){
    string tmp = enc;
    int padd_val = 8 - (enc.length() % 8);
    for (int i = 0; i < padd_val;i++){
        tmp += '0'; //padding zeros at the end 
    }
    string pad_bef = dec_bin(padd_val);
    return pad_bef + tmp;
}
vector<int> get_byte(string enc){
    int i = 0;
    vector<int> res;
    while(i<enc.length()){
        res.push_back(bin_dec(enc.substr(i, 8)));
        i += 8;
    }
    return res;
}
int main()
{
    /*
    vector<pair<char,int>> chars = {{'a',30},{'d',40},{'e',80},{'f',60}};
    */
    string lines;
    string line1;
    ifstream file_("input.txt");
    if(file_.is_open()){
        while(getline(file_,line1)){
            lines += line1;
            //cout << line1 << endl;
        }
        cout << lines<<endl;
        file_.close();
    }
    else{
        cout << "File is not open";
    }
    vector<pair<char,int>> freqs = get_freqs(lines);
    /*sort(freqs.begin(), freqs.end());
    for(auto i:freqs){
        cout<<i.second<<":"<<i.first<<endl;
    }*/
    root = huffmann_encoding(freqs);
    unordered_map<char, string> mp;
    unordered_map<string, char> mp_rev;
    get_codes(root, "",mp,mp_rev);
    string enc = "";
    for(int i=0;i<lines.length();i++){
        enc += mp[lines[i]];
    }
    string orig = enc;
    enc = padding(enc);
    //cout << enc << endl;
    vector<int> bytes_arr = get_byte(enc);
    string cmp = "";
    for(auto i:bytes_arr){
        char tmp = i;
        cmp += tmp;
        //cout << tmp << " ";
    }
    ofstream out_("compress.txt");
    if(out_.is_open()){
        out_ << cmp;
        out_.close();
    }
    else{
        cout << "Failed to open the file";
    }
    /******************************************************************************************************************/
    string cmp_ip;
    string indv_line;
    ifstream cmp_file_("compress.txt");
    if(cmp_file_.is_open()){
        while(getline(cmp_file_,indv_line)){
            cmp_ip += indv_line;
            //cout << line1 << endl;
        }
        //cout << cmp_ip<<endl;
        cmp_file_.close();
    }
    else{
        cout << "File is not open";
    }
    string bin_enc = "";
    for (int i = 0; i < cmp_ip.length();i++){
        int tt = cmp_ip[i];
        bin_enc += dec_bin(tt);
    }
    string init = bin_enc.substr(0, 8); // to get the padding length
    string rem = enc.substr(8);
    int len = bin_dec(init); //padded length
    rem = rem.substr(0, rem.length() - len);
    string dec;
    dec = decode_string_just_tree(root,rem);
    ofstream out2_("output2.txt"); //reate a new output file to put out the decompressed file into it
    if(out2_.is_open()){
        out2_ << dec; //put it in a single line
        out2_.close();
    }
    else{
        cout << "Failed to open the file";
    }
    return 0;
}