//store
import {createStore,combineReducer,apppliMiddware} from 'redux'

const reducers = combineReducer({number:numberReducer,err:errorReducer})

let appState = {number :1 ,history:[1],errorMsg:''}

//reducer 
const numberReducer = (state= appState,action)=>{
    switch (action.type) {
        case 'Add':
            const newValue = state.number+action.value;
            state = {...state,newValue}
            break;
        case 'Rem':
            break;
            const newVal = state.number-action.value;
            state = {...state,newVal}
        default:
            break;
    }
    return state
}

const errorReducer = (state = appState,action) =>{
    switch (action.type) {
        case "Error":
            state = {...state,errorMsg:'Loi roi'}
        case "Less_Than_zero":
            state = {...state,errorMsg:'Loi zero'}
            
            break;
    
        default:
            break;
    }
    return state;
}

//Midderware
const logger =  store=>next=>action =>{
    console.log("State chua up",store.getState())
    next(action);
    console.log('State updated')

}

const checkIsZerro = store=>next=>action =>{
    const currentNubmer = store.getState().number.number;
    if(currentNubmer ==0){
        next({type:"Less_Than_zero"});
    }else 
        {
            next(action)
        }
}

var store = createStore(combineReducer,apppliMiddware(logger,checkIsZerro))

const add = {type :'Add', value :2}
const sub = {type :'Sub', value :3}

store.dispatch(add)
store.dispatch(add)
store.dispatch(sub)