#include <bits/stdc++.h>
#define CLR 4
#define A 3
#define W (10)
#define M 1000000007ll
using i64= int;
#define S (43)
template<typename T>
using vc= std::vector<T>;
using tmat= vc<vc<i64>>;
#define forn(i,n) for(i=0;i<n;++i)

std::istream &is = std::cin;
std::ostream &os = std::cout;

vc<unsigned> vx;
bool se[0x400];

tmat mult(const tmat &x, const tmat &y) {
    tmat m=x,tmp=x;
    int i,j,k,N=x.size();
    assert( x.size() == y.size() );
    for(i=0;i<N;++i)
        for(j=0;j<N;++j)
            for(tmp[i][j]=0,k=0;k<N;++k) {
                auto t=(int64_t)(x[i][k])*y[k][j];
                if(t>=M)
                    t%=M;
                if((tmp[i][j]+=t)>=M)
                    tmp[i][j]-=M;
            }
    for(i=0;i<N;++i)
        for(j=0;j<N;++j)
            m[i][j]= tmp[i][j];
    return m;
}

tmat im( int n ) {
    tmat m;
    m.resize(n,vc<i64>(n,0));
    for (int i=0; i<n; ++i)
        m[i][i]=1;
    return m;
}

vc<tmat> C,prt[A],D[A];
#define B(k) (1ull<<(k))
#define MK(k) (B(k)-1ull)

tmat mpw(tmat &m,i64 n) {
    auto ax= im(m.size()),x= m;
    for(;n;n>>=1,x=mult(x,x))
        if(n&1)ax=mult(ax,x);
    return ax;
}

unsigned char which[1<<11];
#define L(k) ((k) & ((~(k))+1ul))

tmat &get_d( int t, int i ) {
    if ( D[t][i].empty() ) {
        get_d(t,i-1);
        D[t][i] = mult(D[t][i-1],D[t][i-1]);
    }
    return D[t][i];
}

tmat &get_p( int t, unsigned u ) {
    if ( prt[t][u].empty() ) {
        assert( u >= 1 );
        auto v=u&~L(u);
        assert( v < u );
        prt[t][u]=mult(get_p(t,v),get_d(t,which[L(u)]));
        assert( not prt[t][u].empty() );
    }
    return prt[t][u];
}

int ga(unsigned n){
    auto res= get_p(0,n&MK(W));
    for(int i=1;i<A and(n>>=W);++i)
        res=mult(res,get_p(i,n&MK(W)));
    i64 ans=0;
    for(int i=0;i<S;++i)
        for(int j=0;j<S; ++j)
            if((ans+=res[i][j])>=M)
                ans-=M;
    return ans;
}

std::unordered_map<unsigned,int> cm;
#define compat(i,j,k) (((i)!=(j)and(j)!=(k)and(k)!=(i))or((i)*(j)*(k)==0))

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int i, j, k, t, ts, n;
    for (i = 0; i < 11; ++i)
        which[1 << i] = i;
    vx.reserve(S);
    forn(i,CLR) forn(j,CLR) forn(k,CLR)
                if (compat(i,j,k)){
                    unsigned u = (i | (j << 2) | (k << 4));
                    if (not se[u])
                        vx.emplace_back(u), se[u]= true;
                }
    vc<vc<int>> G(S,vc<int>(S,0));
    forn(i,S)forn(j,S)
        G[i][j]=G[i][j]||((vx[i]&0xf)==(vx[j]>>2));
    auto tx= mpw(G,3);
    forn(i,S)forn(j,S)
        if(tx[i][j]==1){
            unsigned u=i|(j<<6);
            if (not cm.count(u)) {
                auto s_= cm.size();
                cm[u]= s_;
            }
        }
    //os << cm.size() << std::endl;
    //return 0;

    tmat mtx(vx.size(), vc<i64>(S, 0));
    assert(vx.size() == S);
    for (i = 0; i < vx.size(); ++i)
        for (j = 0; j < mtx[i].size(); ++j)
            if ((vx[i] & 0xf) == (vx[j] >> 2))
                mtx[i][j] = 1;
    for (C.resize(A), C[0] = mtx, i = 1; i < A; C[i] = mpw(C[i-1],1u<<W), ++i);
    for (t = 0; t < A; ++t)
        D[t].resize(W), D[t][0] = C[t];

    for (t = 0; t < A; ++t)
        prt[t].resize(B(W)), prt[t][0] = im(S);

    for (is>>ts;ts--;){
        is >> n;
        if ( n == 0 )
            os<<1<<'\n';
        else if ( n==1 )
            os<<CLR<<'\n';
        else if (n==2)
            os<<CLR*CLR<<'\n';
        else
            os<<ga(n-3)<<'\n';
    }
    return 0;
}
    