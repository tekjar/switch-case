#!/bin/bash

if [ "$TRAVIS_REPO_SLUG" == "kteza1/switch-case" ] && [ "$TRAVIS_PULL_REQUEST" == "false" ] && [ "$TRAVIS_BRANCH" == "master" ]; then

  # echo -e $TRAVIS_BUILD_DIR
  # echo $HOME
  echo -e "Publishing docs to github pages...\n"

  mkdir -p $HOME/doc-latest 
  cp -R $TRAVIS_BUILD_DIR/documentation/site/* $HOME/doc-latest
  cp -R $TRAVIS_BUILD_DIR/doxygen/html/* $HOME/doc-latest/doxygen

  cd $HOME
  git config --global user.email "travis@travis-ci.org"
  git config --global user.name "travis-ci"
  git clone --quiet --branch=gh-pages https://${GH_TOKEN}@github.com/kteza1/switch-case gh-pages > /dev/null

  cd gh-pages
  git rm -rf *
  cp -Rf $HOME/doc-latest/* .
  git add -f .
  git commit -m "Lastest doc on successful travis build $TRAVIS_BUILD_NUMBER auto-pushed to gh-pages"
  git push -fq origin gh-pages > /dev/null

  echo -e "Published docs to gh-pages.\n"
  
fi
