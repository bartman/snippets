#ifndef _PRIME_GEN_
#define _PRIME_GEN_

#define BAR "+-----------------------------+"
#define BASE_CASE 2
#define FALSE 1

class Prime_Generator
{
 private:
	int req, size;
	char * temp;
	
 public:
	Prime_Generator (const int argument);
	~Prime_Generator (void);
	
	int get_size(void) const;
	char get_temp(const int i) const;
	
	void set_size(const int s);
	void set_temp(const int i);

	void cat(const int req);
};

#endif
