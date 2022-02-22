/******************************************************************************/

/* N++ : C++ class neural network simulator                                   */
/* (c) 1994 Martin Riedmiller                                                 */
/* last changed: 27.7.94                                                      */

/* File: functions.c                                                          */
/* Purpose:                                                                    */
/* (User defined) activation functions, propagation functions,                 */
/* weight update functions,...                                                 */
/* included in net.cc                                                          */

/******************************************************************************/

/* macros: */
#define FORALL_WEIGHTS(WPTR)\
  int Counter;\
  for(Counter=topo->in_count+1;Counter<=topo->unit_count;Counter++)\
    for (WPTR=unit[Counter].weights; WPTR!=NULL; WPTR=WPTR->next)



/***************************************************************/
/*          WEIGHT UPDATE FUNCTIONS                            */
/***************************************************************/

void bp_init(float *params,unit_typ unit[],topo_typ *topo)
{
  weight_typ *wptr;
  
  FORALL_WEIGHTS(wptr){
    wptr->delta = wptr->dEdw = (FTYPE) 0;
  }
}

void bp_update(unit_typ unit[],topo_typ *topo,float *params)
/* update the weights according to gradient descent rule */
{
  weight_typ *wptr;
  float learnrate = params[0];
  float momentum = params[1];
  float wd = params[2];        /* weight_decay */

  FORALL_WEIGHTS(wptr){
    wptr->delta = - learnrate * wptr->dEdw + momentum * wptr->delta - wd * wptr->value; 
    wptr->value += wptr->delta;
    wptr->dEdw = (FTYPE) 0;  /* important: clear dEdw !*/
  }
}

#define UPDATE_VALUE 0.1
#define ETAPLUS 1.2
#define ETAMINUS 0.5
#define DELTA_MIN 1E-6
#define DELTA_MAX 50
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

void rprop_init(float *params,unit_typ unit[],topo_typ *topo)
{
  weight_typ *wptr;
  
  if (! params[0])
    params[0] = UPDATE_VALUE;  /* default */
  if (! params[1])
    params[1] = DELTA_MAX;                    /* default */
  if(params[0]>params[1])
    params[0] = params[1];
  FORALL_WEIGHTS(wptr){
    wptr->delta = wptr->dEdw = (FTYPE) 0;
    wptr->variable[0]= params[0];
  }
}

void rprop_update(unit_typ unit[],topo_typ *topo,float *params)
{
  register float direction, update_value, dEdw;
  weight_typ *wptr;
  register float delta_max = params[1];
  register float wd = params[2];

  FORALL_WEIGHTS(wptr){
    update_value = wptr->variable[0];
    dEdw =  wptr->dEdw + wd * wptr->value;
    direction = wptr->delta * dEdw;
    if(direction<0.0){
      update_value = MIN(update_value * ETAPLUS,delta_max);
      if(dEdw>0.0)
	wptr->delta = - update_value;
      else /* dEdw<0.0 */
	wptr->delta = update_value;
    }
    else if(direction>0.0){
      update_value = MAX(update_value * ETAMINUS,DELTA_MIN);
      wptr->delta = 0.0;  /* restart adaptation in next step */
    }
    else{ /* direction == 0.0 */
      if(dEdw>0.0)
	wptr->delta = - update_value;
      else if(dEdw<0.0)
	wptr->delta = update_value;
      else
	wptr->delta = (FTYPE) 0;
    }
    wptr->value += wptr->delta;
    wptr->variable[0] = update_value;
    wptr->dEdw = (FTYPE) 0;  /* important: clear dEdw !*/
  }
}

/***************************************************************/
/*          ACTIVATION FUNCTIONS                               */
/***************************************************************/

FTYPE logistic(unit_typ *unit_i)
{
  FTYPE net_in = unit_i->net_in;
  if (net_in >= 16.0) net_in = 16.0;     /* avoid under/overflow */
  if (net_in <= - 16.0) net_in = -16.0;
  return( (FTYPE) (1.0 / (1.0 + exp( -net_in))) );
}

FTYPE logistic_deriv(unit_typ *unit_i)
{
  return((1.0 - unit_i->out) * unit_i->out);
}

FTYPE symmetric(unit_typ *unit_i)
{
  FTYPE net_in = unit_i->net_in;
  if (net_in >= 7.0) net_in = 7.0;     /* avoid under/overflow */
  if (net_in <= - 7.0) net_in = -7.0;
  return( (FTYPE) (-1.0 + 2.0/(1.0 + exp(- net_in * 2.0)))  );
}

FTYPE symmetric_deriv(unit_typ *unit_i)
{
  return(1.0 - (unit_i->out * unit_i->out));
}

FTYPE linear(unit_typ *unit_i)
{
  return(unit_i->net_in);
}

FTYPE linear_deriv(unit_typ *unit_i)
{
  return(1.0);
}

/***************************************************************/
/*          PROPAGATION FUNCTIONS                              */
/***************************************************************/

void prop_forward(unit_typ unit[],topo_typ *topo)
{
  int i;
  weight_typ *wptr;
  
  for(i=topo->in_count+1;i<=topo->unit_count;i++){
    unit[i].net_in = (FTYPE) 0;  /* reset net in */
    for (wptr=unit[i].weights; wptr!=NULL; wptr=wptr->next) /* compute new net in */
      unit[i].net_in += wptr->value* unit[wptr->from_unit].out;
    unit[i].out = unit[i].act_f(&unit[i]); /* compute unit activation and output */
  }
}

void prop_backward(unit_typ unit[],topo_typ *topo)
/* propagate backwards, compute dEdw and(!) dE/d(input) */
{
  int i;
  weight_typ *wptr;

  for(i=topo->unit_count;i>0;i--){
    unit[i].dEdnet = unit[i].dEdo * unit[i].deriv_f(&unit[i]); /* de/dnet_i */
    unit[i].dEdo = (FTYPE) 0;  /* clear for next time */
    for (wptr=unit[i].weights; wptr!=NULL; wptr=wptr->next){
      wptr->dEdw += unit[i].dEdnet * unit[wptr->from_unit].out;   /* de/dwij */
      unit[wptr->from_unit].dEdo += unit[i].dEdnet * wptr->value; /* de/do_j */
    }
  }
}

void do_backward_pass_light(unit_typ unit[],topo_typ *topo)
/* propagate backwards, compute ONLY!! dE/d(input) */
{
  int i;
  weight_typ *wptr;

  for(i=topo->unit_count;i>0;i--){
    unit[i].dEdnet = unit[i].dEdo * unit[i].deriv_f(&unit[i]); /* de/dnet_i */
    unit[i].dEdo = (FTYPE) 0;  /* clear for next time */
    for (wptr=unit[i].weights; wptr!=NULL; wptr=wptr->next){
      unit[wptr->from_unit].dEdo += unit[i].dEdnet * wptr->value; /* de/do_j */
    }
  }
}
