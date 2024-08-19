import React from "react";
import axios from 'axios';
import 'bootstrap/dist/css/bootstrap.min.css';
import ListGroup from 'react-bootstrap/ListGroup';
import Card from 'react-bootstrap/Card';
import '../src/App.css';

export default class PersonList extends React.Component {
  state = {
    persons: []
  }

  componentDidMount() {
    axios.get('http://10.106.23.35:5555/getAllData')
      .then(res => {
        const persons = res.data;
        
        this.setState({ persons });
      })
      .catch(error => console.log(error));
  }

  render() {
    return (
      <ul>
        { this.state.persons.map(person => 
          <div>
            <Card className = 'card'>
              <Card.Header>
                <Card.Title className = 'title'><h3>Thong tin nhiet do </h3></Card.Title>
              </Card.Header>
              <Card.Img className = "anh" variant = "top" src = 'https://img.freepik.com/free-vector/hand-drawn-flat-design-stack-books-illustration_23-2149341898.jpg?w=360'/>
              <ListGroup className = 'text'>
                <ListGroup.Item>ID: {person.id} </ListGroup.Item>
                <ListGroup.Item>Device: {person.device} </ListGroup.Item>
                <ListGroup.Item>Temperature: {person.temperature}</ListGroup.Item>
                
              </ListGroup>
            </Card>
          </div>)}
      </ul>      
    )
  }
}