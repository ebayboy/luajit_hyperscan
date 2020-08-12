
#include <cstdio>

#ifdef __cplusplus
extern "C" {
#endif
int fast(int *a,int m,int n)
{  
	int key;
	int i,j;
	if(m<n){
		i=m;
		j=n;
		key=a[i];
		while(i<j){							
			while(i<j && a[j]>key){			
				j--;
			}
			if(i<j){
				a[i++]=a[j];				
				//i++;
			}
			while(i<j && a[i]<key){			
				i++;
			}
			if(i<j){
				a[j--]=a[i];				
				//j--;
			}
		}
		a[i]=key;
		fast(a,m,i-1);						
		fast(a,i+1,n);						
	}

	return 0;
}

#ifdef __cplusplus
}
#endif
