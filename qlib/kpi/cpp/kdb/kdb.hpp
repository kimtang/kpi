
/** @mainpage KDB Library 
 *  
 *  @section sec1 Introduction
 *  This library is created to provide a framework to interact with kdb+ under c++.
 *  We implemented 2 abstractions level. In the first level we have functions to convert q-types into c++-types
 *  On the second level we have functions to convert lists from kdb+ into vectors on the c++ level.
 * 
 *  @section sec2 Todo:
 *  @section sec3 Todo:
 */


# ifndef KDB_HPP_KKT_11_05_2014
# define KDB_HPP_KKT_11_05_2014

# define BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG

# include <kdb/k.h>
# include <stdexcept>
# include <string>
# include <boost/mpl/void.hpp>
# include <boost/mpl/identity.hpp>
# include <boost/mpl/negate.hpp>
# include <boost/mpl/eval_if.hpp>
# include <boost/utility/result_of.hpp>
# include <boost/preprocessor/tuple.hpp>
# include <boost/preprocessor/seq.hpp>
# include <boost/date_time.hpp>
# include <boost/iterator/iterator_facade.hpp>

# include <boost/uuid/uuid.hpp>

/*
	To ease the the interfacing in c with kdb+ we implement a wrapper around the kx structure 

*/

/* Here we should define a table
	qtype			| type	| bytes | ctype			| accessors | rtype
------------------------------------------------------------------------
	boolean			| -KB	| 1		| char			| kG		| bool
	byte			| -KG	| 2		| char			| kG		| byte
	short			| -KH	| 2		| short			| kH		| short
	int				| -KI	| 4		| int			| kI		| int
	long			| -KJ	| 8		| long long		| kJ		| long long
	real			| -KE	| 4		| float			| kE		| float
	float			| -KF	| 8		| double		| kF		| double
	char			| -KC	| 1		| char			| kC		| char
	symbol			| -KS	| null	| char*			| kS		| std::string
	timestamp		| -KP	| 8		| long long		| kJ		| long long
	month			| -KM	| 4		| int			| kI		| int
	date			| -KD	| 4		| int			| kI		| int
	datetime		| -KZ	| 8		| double		| kF		| double
	timespan		| -KN	| 8		| long long		| kJ		| long long
	minute			| -KU	| 4		| int			| kI		| int
	second			| -KV	| 4		| int			| kI		| int	
	time			| -KT	| 4		| int			| kI		| int

# define KB 1  // 1 boolean  char   kG
# define KG 4  // 1 byte     char   kG
# define KH 5  // 2 short    short  kH
# define KI 6  // 4 int      int    kI
# define KJ 7  // 8 long     int64  kJ
# define KE 8  // 4 real     float  kE
# define KF 9  // 8 float    double kF
# define KC 10 // 1 char     char   kC
# define KS 11 // * symbol   char*  kS
# define KP 12 // 8 timestampint64  kJ (nanoseconds from 2000.01.01)
# define KM 13 // 4 month    int    kI
# define KD 14 // 4 date     int    kI (days from 2000.01.01)
# define KZ 15 // 8 datetime double kF (days from 2000.01.01)
# define KN 16 // 8 timespan int64  kJ
# define KU 17 // 4 minute   int    kI
# define KV 18 // 4 second   int    kI
# define KT 19 // 4 time     int    kI (millisecond)
*/



# define kdb_dim 9

// 
// 0			,1		  ,2  ,3  ,4    ,5 ,6                  ,7                               ,8 
// name			,type(int),get,ctk,getL,   ,                   ,ktc
# define kdb_table																																			\
((boolean		,kx_KB,get_boolean,G,kx_kG,G*,	ktc_boolean,bool							,ctk_boolean		))									\
((short			,kx_KH,get_short,H,kx_kH,H*,	ktc_short,short								,ctk_short			))									\
((int			,kx_KI,get_int,I,kx_kI,I*,		ktc_int,int									,ctk_int			))									\
((long			,kx_KJ,get_long,J,kx_kJ,J*,		ktc_long,long long							,ctk_long			))									\
((real			,kx_KE,get_real,E,kx_kE,E*,		ktc_real,float								,ctk_real			))									\
((float			,kx_KF,get_float,F,kx_kF,F*,	ktc_float,double							,ctk_float			))									\
((symbol		,kx_KS,get_symbol,S,kx_kS,S*,	ktc_symbol,char*							,ctk_symbol			))									\
((timestamp		,kx_KP,get_timestamp,J,kx_kJ,J*,ktc_timestamp,boost::posix_time::ptime		,ctk_timestamp		))									\
((date			,kx_KD,get_date,I,kx_kI,I*,		ktc_date,boost::gregorian::date				,ctk_date			))									\
((datetime		,kx_KZ,get_datetime,F,kx_kF,F*,	ktc_datetime,boost::posix_time::ptime		,ctk_datetime		))									\
((char			,kx_KC,get_char,G,kx_kC,G*,		ktc_char,char								,ctk_char			))									\
((list			,kx_KK,get_K,K,kx_kK,K*,		ktc_K,K										,ctk_K				))									\


# define kdb_qtype(r,data,tuple)																								\
struct BOOST_PP_TUPLE_ELEM(kdb_dim,0,tuple)##_																					\
{																																\
	typedef mpl::int_< BOOST_PP_TUPLE_ELEM(kdb_dim,1,tuple) > type;																\
	typedef typename mpl::negate<mpl::int_< BOOST_PP_TUPLE_ELEM(kdb_dim,1,tuple) > >::type atom_type;							\
	typedef BOOST_PP_TUPLE_ELEM(kdb_dim,0,tuple)##_ this_type;																	\
	typedef BOOST_PP_TUPLE_ELEM(kdb_dim,7,tuple) value_type;																	\
																																\
																																\
	BOOST_PP_TUPLE_ELEM(kdb_dim,7,tuple) ktc(BOOST_PP_TUPLE_ELEM(kdb_dim,3,tuple)  x) const										\
	{																															\
		return BOOST_PP_TUPLE_ELEM(kdb_dim,6,tuple) (x);																				\
	}																															\
																																\
	BOOST_PP_TUPLE_ELEM(kdb_dim,3,tuple) ctk(BOOST_PP_TUPLE_ELEM(kdb_dim,7,tuple)  x) const										\
	{																															\
		return BOOST_PP_TUPLE_ELEM(kdb_dim,8,tuple) (x);																				\
	}																															\
																																\
	BOOST_PP_TUPLE_ELEM(kdb_dim,3,tuple) get(K k) const																			\
	{																															\
		return BOOST_PP_TUPLE_ELEM(kdb_dim,2,tuple)(k);																		\
	}																															\
																																\
	BOOST_PP_TUPLE_ELEM(kdb_dim,3,tuple)* getL(K k) const																		\
	{																															\
		return BOOST_PP_TUPLE_ELEM(kdb_dim,4,tuple)(k);																			\
	}																															\
	BOOST_PP_TUPLE_ELEM(kdb_dim,7,tuple) value(K k) const																		\
	{																															\
		return this->ktc(this->get(k));																							\
	}																															\
																																\
};																																\


# define kdb_result_of(r,data,tuple)																							\
template<> struct get<qtype::BOOST_PP_TUPLE_ELEM(kdb_dim,0,tuple)##_ >															\
{																																\
	typedef BOOST_PP_TUPLE_ELEM(kdb_dim,3,tuple) type;																			\
};																																\

# define kdb_result_ofL(r,data,tuple)																							\
template<> struct getL<qtype::BOOST_PP_TUPLE_ELEM(kdb_dim,0,tuple)##_ >															\
{																																\
	typedef BOOST_PP_TUPLE_ELEM(kdb_dim,5,tuple) type;																			\
};																																\

# define kdb_result_of_v(r,data,tuple)																							\
template<> struct value<qtype::BOOST_PP_TUPLE_ELEM(kdb_dim,0,tuple)##_ >														\
{																																\
	typedef BOOST_PP_TUPLE_ELEM(kdb_dim,7,tuple) type;																			\
};																																\

# define kdb_result_of_ktc(r,data,tuple)																						\
template<> struct ktc<qtype::BOOST_PP_TUPLE_ELEM(kdb_dim,0,tuple)##_ >															\
{																																\
	typedef BOOST_PP_TUPLE_ELEM(kdb_dim,7,tuple) type;																			\
};																																\

# define kdb_result_of_ctk(r,data,tuple)																						\
template<> struct ctk<qtype::BOOST_PP_TUPLE_ELEM(kdb_dim,0,tuple)##_ >															\
{																																\
	typedef BOOST_PP_TUPLE_ELEM(kdb_dim,3,tuple) type;																			\
};																																\

namespace kx { 
namespace mpl = boost::mpl;
namespace qtype{

	boost::gregorian::date const gday = boost::gregorian::date(2000, boost::gregorian::Jan, 1); // used for kdb to convert the number in boost date time
	boost::gregorian::date const bday = boost::gregorian::date(2000, boost::gregorian::Jan, 1); // used for kdb to convert the number in boost date time
	boost::posix_time::ptime const pday = boost::posix_time::ptime(gday, boost::posix_time::time_duration(0, 0, 0, 0)); // used for kdb to convert the number in boost date time

	bool ktc_boolean(G x) {return 0 != x;}
	short ktc_short(H x) {return static_cast<short>(x);}
    int ktc_int(I x) { return static_cast<int>(x); }
	long long ktc_long(J x){return static_cast<long long>(x);}
	float ktc_real(E x){return static_cast<float>(x);}
	double ktc_float(F x){return static_cast<double>(x);}
	char* ktc_symbol(S x){return x;}
	boost::gregorian::date ktc_date(I x){return bday+boost::gregorian::days(x);}
	char ktc_char(G x){return static_cast<char>(x);}
	K ktc_K(K x){return x;}

	boost::posix_time::ptime ktc_timestamp(J t) {
		kx::J nnans = t % 1000000000;
		t = (t - nnans) / 1000000000;
		kx::J nsecs = t % 60;
		t = (t - nsecs) / 60;
		kx::J nmins = t % 60;
		t = (t - nmins) / 60;
		kx::J nhrs = t % 24;
		t = (t - nhrs) / 24;
		boost::posix_time::ptime p = kx::qtype::pday
			+ boost::gregorian::days(static_cast<int>(t))
			+ boost::posix_time::time_duration(static_cast<int>(nhrs), static_cast<int>(nmins), static_cast<int>(nsecs))
			+ boost::posix_time::nanosec(nnans);
		return p;
	}
	boost::posix_time::ptime ktc_datetime(F x) {
		long long t = static_cast<long long> (x * 86400000.0);
		long long millisecs = t % 86400000;
		long long days = (t - millisecs) / 86400000;
		return pday + boost::gregorian::days(static_cast<int>(days)) + boost::posix_time::millisec(millisecs);
	}

	G ctk_boolean(bool x) { return static_cast<kx::G>(x); }
	H ctk_short(short x) { return static_cast<kx::H>(x); }
	I ctk_int(int x) { return static_cast<kx::I>(x); }
	J ctk_long(long long x) { return static_cast<kx::J>(x); }
	E ctk_real(float x) { return static_cast<kx::E>(x); }
	F ctk_float(double x) { return static_cast<kx::F>(x); }
	S ctk_symbol(char* x) { return ss(x); }
	I ctk_date(boost::gregorian::date x) { return (gday - x).days(); }
	G ctk_char(char x) { return static_cast<kx::G>(x); }
	K ctk_K(K x) { return x; }
	J ctk_timestamp(boost::posix_time::ptime x) {
		kx::J j = x.time_of_day().total_nanoseconds();
		j += (x.date() - gday).days() * 86400000000000L;
		return j;
	}

	F ctk_datetime(boost::posix_time::ptime x) {
		kx::F f = x.time_of_day().total_milliseconds() / 86400000.0;
		f += (x.date() - gday).days();
		return f;
	}

	G get_boolean(K k) { return  k->g; }
	H get_short(K k) { return  k->h; }
	I get_int(K k) { return  k->i; }
	J get_long(K k) { return  k->j; }
	E get_real(K k) { return  k->e; }
	F get_float(K k) { return  k->f; }
	S get_symbol(K k) { return  k->s; }
	J get_timestamp(K k) { return  k->j; }
	I get_date(K k) { return  k->i; }
	F get_datetime(K k) { return  k->f; }
	G get_char(K k) { return  k->g; }
	K get_K(K k) { return  k->k; }


	BOOST_PP_SEQ_FOR_EACH(kdb_qtype,~,kdb_table)

	/* Todo:
	We need to define the type list, map and table
	*/

} // qtype

namespace result_of
{
	template<typename Q> struct get;
	BOOST_PP_SEQ_FOR_EACH(kdb_result_of,~,kdb_table)

	template<typename Q> struct getL;
	BOOST_PP_SEQ_FOR_EACH(kdb_result_ofL,~,kdb_table)

	template<typename Q> struct value;
	BOOST_PP_SEQ_FOR_EACH(kdb_result_of_v,~,kdb_table)

	template<typename Q> struct ktc;
	BOOST_PP_SEQ_FOR_EACH(kdb_result_of_ktc,~,kdb_table)

	template<typename Q> struct ctk;
	BOOST_PP_SEQ_FOR_EACH(kdb_result_of_ctk,~,kdb_table)
}

template<typename Q> 
typename result_of::get<Q>::type get(K k) {return Q().get(k);};

template<typename Q> 
typename result_of::getL<Q>::type getL(K k) {return Q().getL(k);};

template<typename Q> 
typename result_of::value<Q>::type value(K k) {return Q().value(k);};

template<typename Q,typename T> 
typename result_of::ktc<Q>::type ktc(T & t) {return Q().ktc(t);};

template<typename Q,typename T> 
typename result_of::ctk<Q>::type ctk(T t) {return Q().ctk(t);};

//namespace detail{
//
//	template<typename T> struct getType {};
//
//	template<> 
//	struct getType<qtype::boolean_> 
//	{
//		typedef mpl::int_<kx_KB> type;
//	};
//
//} // detail

template<typename Q>
struct reference
{
public:
	typedef typename reference<Q> this_type;
	typedef typename result_of::getL<Q>::type qtype_;
	typedef typename mpl::eval_if<typename boost::is_const<Q>::type, boost::add_const<qtype_>,mpl::identity<qtype_> >::type qtype;
	typedef typename result_of::value<Q>::type value_type;
	 reference() : q_(0) {};
	 reference(qtype q): q_(q) {};
	this_type& operator=(value_type const& o) 
	{ 
		(*q_)= Q().ctk(o);
		return *this;
	};
	operator value_type () const { return ktc<Q>(*q_); };
	value_type const operator ()() const { return ktc<Q>(*q_); };
	friend std::ostream &operator<<(std::ostream &sout, this_type const& r)
	{
		return sout<<r();
	};
private:
	qtype q_;
};

template<typename Q>
class iterator 
	: public boost::iterator_facade<
		  iterator <Q> // Derived
		, typename result_of::value<Q>::type
		, std::random_access_iterator_tag
//		, boost::random_access_traversal_tag
		, reference<Q> // Here we need to choose, if the c-type and k-type doesnt match, then we need to cast or convert it.
	>
{
public:
	typedef typename result_of::getL<Q>::type qtype_;
	typedef typename mpl::eval_if<
		  typename boost::is_const<Q>::type
		, boost::add_const<qtype_>,mpl::identity<qtype_> 
	>::type qtype;
	typedef iterator<Q> this_type;
	typedef typename this_type::difference_type difference_type;
	typedef typename this_type::reference reference;
	explicit iterator(qtype q) : q_(q) {};
	iterator() : q_(0) {};
	qtype const operator()() const {return q_;};
	reference operator[] (difference_type const& size) const{ return reference(q_ + size); }

private:
	friend class boost::iterator_core_access;
	qtype q_;
	void increment(){ ++q_; }
	void decrement(){ --q_; }
	void advance(difference_type n) {q_=q_+n;}
    bool equal(this_type const& o) const { return o() == this->q_; }
	difference_type distance_to(this_type const& o) const { return std::distance(this->q_,o());} 
	reference dereference () const { return reference(q_); }
};

template<typename Q>
class reverse_iterator 
	: public boost::iterator_facade<
		  iterator <Q> // Derived
		, typename result_of::value<Q>::type
		, boost::random_access_traversal_tag
		, reference<Q> // Here we need to choose, if the c-type and k-type doesnt match, then we need to cast or convert it.
	>
{
public:
	typedef typename result_of::getL<Q>::type qtype_;
	typedef typename mpl::eval_if<
		  typename boost::is_const<Q>::type
		, boost::add_const<qtype_>,mpl::identity<qtype_> 
	>::type qtype;
	typedef iterator<Q> this_type;
	typedef typename this_type::difference_type difference_type;
	typedef typename this_type::reference reference;
	explicit reverse_iterator(qtype q) : q_(q) {};
	reverse_iterator() : q_(0) {};
	qtype const operator()() const {return q_;};
	reference operator[] (difference_type const& size) const{ return reference(q_ - size); }

private:
	friend class boost::iterator_core_access;
	qtype q_;
	void increment(){ --q_; }
	void decrement(){ ++q_; }
	void advance(difference_type n) {q_=q_ - n;}
    bool equal(this_type const& o) const { return o() == this->q_; }
	difference_type distance_to(this_type const& o) const { return std::distance(this->q_,o());}
	reference dereference () const { return reference(q_); }
};

template<typename Q>
class vector
{
public:
	typedef vector<Q> this_type;
	typedef std::size_t size_type;
	typedef typename result_of::value<Q>::type value_type;
	typedef typename value_type* ptr_type;
	typedef typename value_type const* const_ptr_type;

	typedef typename Q::type q_id;
	typedef kx::iterator<Q> iterator;
	typedef kx::iterator<Q const> const_iterator;
	typedef kx::reverse_iterator<Q> reverse_iterator;
	typedef kx::reverse_iterator<Q const> const_reverse_iterator;

	vector(K k) : k_(k),size_(0)
	{
		if(q_id()!=std::abs(k_->t) || k_==0)
			; // if the data is violated, do nothing
		else
		{
			std::int8_t t = static_cast<std::int8_t>(k_->t);
			if (t == 0)
			{
				size_ = static_cast<size_type>(k_->n);
				k_ = kx::d9(kx::b9(-1, k_));
			}

			if (t > 0)
			{
				size_ = static_cast<size_type>(k_->n);
				k_ = kx::ktn(q_id(), size_);
				typename kx::result_of::getL<Q>::type k0 = kx::getL<Q>(k);
				std::copy(k0, k0 + size_, begin());
			}
			
			if (t < 0)
			{
				k_ = kx::ktn(q_id(),1);
				size_=1;
				std::fill(begin(),end(),value<Q>(k));
			}
		}

	};
	vector() : k_(0),size_(0) {k_ = kx::ktn(q_id(),size_);};
	vector(size_type size) : k_(kx::ktn(q_id(),size)),size_(size) {};
	vector(size_type size,value_type v) : k_(kx::ktn(q_id(),size)),size_(size) {std::fill(begin(),end(),v);};
	vector(ptr_type b, ptr_type e) : k_(kx::ktn(q_id(),e-b)),size_(e-b){std::copy(b,e,begin()); }
	vector(const_ptr_type b, const_ptr_type e) : k_(kx::ktn(q_id(), e - b)), size_(e - b) { std::copy(b, e, begin()); }
	kx::K operator()() {return k_;}
	operator kx::K() {return k_;}
	// here i need an iterator
	size_type size() const {return size_;}
	bool empty() const {return this->size()==0 ;}
	iterator begin() {return iterator( kx::getL<Q>(k_)); }
	iterator end() {return iterator( kx::getL<Q>(k_)+size_); }
	const_iterator begin() const {return const_iterator( kx::getL<Q>(k_)); }
	const_iterator end() const {return const_iterator( kx::getL<Q>(k_)+size_); }
	iterator rbegin() {return reverse_iterator( kx::getL<Q>(k_)+size_-1); }
	iterator rend() {return reverse_iterator( kx::getL<Q>(k_)-1); }
	const_iterator rbegin() const {return const_reverse_iterator( kx::getL<Q>(k_)+size_-1); }
	const_iterator rend() const {return const_reverse_iterator( kx::getL<Q>(k_)-1); }

private: 
	K k_;
	size_type size_;

};

template<typename Q>
class raw_vector
{
public:
	typedef raw_vector<Q> this_type;
	typedef std::size_t size_type;
	typedef typename result_of::value<Q>::type value_type;
	typedef typename Q::type q_id;
	typedef typename kx::result_of::getL<Q>::type iterator;
	typedef typename kx::result_of::getL<Q>::type const const_iterator;

	raw_vector(K k) : k_(k),size_(0)
	{
		if( q_id()==k_->t && k_!=0) size_ = k_->n;
		else k_=0;
	};
	raw_vector() : k_(0),size_(0) {k_ = kx::ktn(q_id(),size_);};
	raw_vector(size_type size) : k_(kx::ktn(q_id(),size)),size_(size) {};
	kx::K operator()() {return k_;}
	operator kx::K() { return k_; }
	kx::K r1() { return kx::r1(k_); }; // explicit increment refcount
	// here i need an iterator
	size_type size() const {return size_;}
	bool empty() const {return this->size()==0 ;}
	iterator begin() {return kx::getL<Q>(k_); }
	iterator end() {return kx::getL<Q>(k_)+size_; }
	const_iterator begin() const {return  kx::getL<Q>(k_); }
	const_iterator end() const {return kx::getL<Q>(k_)+size_; }

private: 
	K k_;
	size_type size_;

};

std::map<std::string, kx::K> k2d(kx::K k_)
{
	std::map<std::string, kx::K> m;
	std::int8_t t0 = static_cast<std::int8_t>(k_->t);
	if (t0 != 99) { return m; }
	// kx::result_of::value<qtype::list_>::type* l_ = kx::getL<qtype::list_>(k_);
	kx::K* l_ = kx::getL<qtype::list_>(k_);
	kx::K k0 = l_[0];
	kx::raw_vector<qtype::list_>v(l_[1]);

	std::int8_t t1 = static_cast<std::int8_t>(k0->t);
	if (t1 != qtype::symbol_::type()) { return m; };

	kx::raw_vector<qtype::symbol_>k(k0);
	kx::raw_vector<qtype::list_>::iterator v1 = v.begin();
	kx::raw_vector<qtype::symbol_>::iterator k1 = k.begin();
	while (v1 != v.end() && k1 != k.end())
	{
		std::string s(*k1);
		m.insert(std::make_pair(s, *v1));
		v1++; k1++;
	}

	return m;
}

std::map<std::string, kx::K> t2d(kx::K k_)
{
	std::map<std::string, kx::K> m;
	std::int8_t t0 = static_cast<std::int8_t>(k_->t);
	if (t0 != 98) { return m; }
	// kx::result_of::value<qtype::list_>::type* l_ = kx::getL<qtype::list_>(k_);
	kx::K* l_ = kx::getL<qtype::list_>(k_->k);
	kx::K k0 = l_[0];
	kx::raw_vector<qtype::list_>v(l_[1]);

	std::int8_t t1 = static_cast<std::int8_t>(k0->t);
	if (t1 != qtype::symbol_::type()) { return m; };

	kx::raw_vector<qtype::symbol_>k(k0);
	kx::raw_vector<qtype::list_>::iterator v1 = v.begin();
	kx::raw_vector<qtype::symbol_>::iterator k1 = k.begin();
	while (v1 != v.end() && k1 != k.end())
	{
		std::string s(*k1);
		m.insert(std::make_pair(s, *v1));
		v1++; k1++;
	}

	return m;
}

kx::K d2d(std::map<std::string, kx::K> & d)
{
	kx::vector<qtype::symbol_>k(d.size());
	kx::vector<qtype::symbol_>::iterator to0 = k.begin();
	kx::K v = kx::knk(0, 0);

	for (std::map<std::string, kx::K>::iterator from0 = d.begin(); from0 != d.end(); ++from0,++to0)
	{
		(*to0) = kx::ss((from0->first).c_str());
		kx::jk(&v, from0->second);
	}

	return kx::xD(k(), v);
}

kx::K d2t(std::map<std::string, kx::K>& d){return kx::xT(d2d(d));}

} // kx

# undef kdb_dim

# undef kdb_boolean
# undef kdb_short
# undef kdb_int
# undef kdb_long
# undef kdb_real
# undef kdb_float
# undef kdb_symbol
# undef kdb_timestampint64
# undef kdb_date
# undef kdb_datetime
# undef kdb_table
# undef kdb_convert

# include <kdb/kundef.hpp>

# endif
