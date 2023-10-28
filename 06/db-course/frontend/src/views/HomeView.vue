<template>
  <main class="container">
    <br>

    <div class="row">
      <div class="col-md-2"></div>
      <div class="col-md-8">
        <div class="card mb-3">
          <div class="card-body">
            <h1>Hello!</h1>

            <p>
              We are pleased to announce the launch of our internal social network — balteca! With its help, you will be
              able to apply for vacations, easily find necessary employees, and keep track of the work of other
              departments in our organization.
            </p>

            <p>
              We are confident that Balteca will become an integral part of our work and help us efficiently solve work
              tasks. You will be able to create profiles, exchange information and opinions, as well as find new friends
              and colleagues.
            </p>

            <p class="mb-0">
              The registration is already open, join our community and follow the updates. Let's work together to make
              our work more convenient and interesting!
            </p>
          </div>
        </div>

        <div class="card mb-2">
          <div class="card-body">
            <h1>Services</h1>

            <a class="btn btn-light me-2" href="/vacations">
              Vacations
            </a>

            <a class="btn btn-light me-2" href="/search">
              Search
            </a>

            <a class="btn btn-light me-2" href="/departments">
              Departments
            </a>

            <a class="btn btn-light me-2" href="/employee">
              Employee
            </a>

            <a class="btn btn-light me-2" href="/teams">
              Teams
            </a>
          </div>
        </div>
      </div>
      <div class="col-md-2"></div>
    </div>
  </main>
</template>

<script lang="ts">
import {defineComponent} from "vue";

export default defineComponent({});
</script>