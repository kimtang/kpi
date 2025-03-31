
# include <kdb/kdb.hpp>

namespace qtype = kx::qtype;

kx::K kpi_api_dot(kx::K a1, kx::K a2)
{
	return kx::dot(a1,a2);
}


# define kdb_dim 9

# define kdb_table																																			\
((boolean				,kx_KB,g,G,kx_kG,G*,	kdb_ktc_boolean,bool							,kdb_ctk_boolean		))									\
((short					,kx_KH,h,H,kx_kH,H*,	kdb_ktc_short,short								,kdb_ctk_short			))									\
((int					,kx_KI,i,I,kx_kI,I*,	kdb_ktc_int,int									,kdb_ctk_int			))									\
((long					,kx_KJ,j,J,kx_kJ,J*,	kdb_ktc_long,long long							,kdb_ctk_long			))									\
((real					,kx_KE,e,E,kx_kE,E*,	kdb_ktc_real,float								,kdb_ctk_real			))									\
((float					,kx_KF,f,F,kx_kF,F*,	kdb_ktc_float,double							,kdb_ctk_float			))									\
((symbol				,kx_KS,s,S,kx_kS,S*,	kdb_ktc_symbol,char*							,kdb_ctk_symbol			))									\
((timestampint64		,kx_KP,j,J,kx_kJ,J*,	kdb_ktc_timestampint64,boost::posix_time::ptime	,kdb_ctk_timestampint64	))									\
((date					,kx_KD,i,I,kx_kI,I*,	kdb_ktc_date,boost::gregorian::date				,kdb_ctk_date			))									\
((datetime				,kx_KZ,f,F,kx_kF,F*,	kdb_ktc_datetime,boost::posix_time::ptime		,kdb_ctk_datetime		))									\
((char					,kx_KC,g,G,kx_kC,G*,	kdb_ktc_char,char								,kdb_ctk_char			))									\

kx::K kpi_api_xn(kx::K x_)
{
	return kx::kj(x_->n);
}

kx::K kpi_api_xt(kx::K x_)
{
	return kx::kc(x_->t);
}

kx::K kpi_api_xp(kx::K x_)
{
	std::uintptr_t ptr = reinterpret_cast<long>(x_);
	return kx::kj(ptr);
}


kx::K kpi_api_boolean0(kx::K x_)
{
	kx::result_of::value<qtype::long_>::type x = kx::value<qtype::long_>(x_);
	return kx::kb(x);
}

kx::K kpi_api_boolean1(kx::K x_)
{
	kx::raw_vector<qtype::boolean_> x(x_);
	return x.r1(); // to reuse the vector in raw_vector again you need to increase the count
}

kx::K kpi_api_boolean2(kx::K x_)
{
	kx::vector<qtype::boolean_> x(x_);
	return x; // the vector is copied
}